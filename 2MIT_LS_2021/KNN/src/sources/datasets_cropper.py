import torch
from scipy.sparse.data import _minmax_mixin
from torch.utils import data

from cropper import Cropper
from couch import Couch, get_classes_in_image, directory, get_origin_name

import cv2
import os
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
from collections import Counter
import sys


class CropperSegmentationDataSet(data.Dataset):
    def __init__(self,
                 couch: Couch,
                 width: int,
                 height: int,
                 transform=None,
                 classes=None,
                 cross_entropy=False,
                 limit=None,
                 save_tensor=False  # set true if you wanna save tensor => memory consumption but velocity
                 ):
        self.couch = couch
        self.cache = CacheManager(width=width, height=height, couch_instance=couch)
        self.inputs_dtype = torch.float32
        self.targets_dtype = torch.long
        """
        This is a list of files (triplet: origin_image_path, classes_image_path, objects_image_path).
            Structure is: [
                (origin_image_path_1, classes_image_path_1, objects_image_path_1),
                (origin_image_path_2, classes_image_path_2, objects_image_path_2),
                .
                .
                .
            ] 
        For each element of this list is true that each element of this triplet is different (there is really class 
        template image, because dataset can contain any origins without class template and it is useless)
        """
        #files = couch.get_triplets_with_class_template(couch.train) + couch.get_triplets_with_class_template(couch.val)
        final_structure, files = self.cache.get_records_and_triplets()

        print("Initialization of files done: {} files".format(len(files)))

        """
        Contains structure of quartet: (origin_img, class_img, object_img, (x, y), transformation), where the last 
        element contains the coords of the crop in the cropper. Transformation is a value of rotation which should be 
        done with image before cropping.
        This is used for iterating over nonexistent cropped images
        """
        self.final_structure = final_structure

        self.cropper = Cropper(width=width, height=height)

        self.init_from_scratch(files)

        print("Initialization of final_structure done: {} records (images to train)".format(len(self.final_structure)))

        self.to_tensor=True
        self.transform=transform
        self.mapper = ColorMapper()
        if classes is None:
            self.classes = list(self.mapper.id_tab.values())
        else:
            self.classes = classes
        self.cross_entropy = cross_entropy
        self.memory = {}
        self.memory_init_on = False
        self.limit = limit
        self.save_tensor = save_tensor
        if limit is not None:
            self.init_memory(from_index=0)

    def init_memory(self, from_index):
        """
        Initialize memory pictures
        """
        for k, v in self.memory.items():
            i1, i2 = v
            cv2.imshow("", i1)
            del i1
            del i2
        self.memory.clear()
        del self.memory
        self.memory = {}
        self.memory_init_on = True
        last_index = len(self) if len(self) < from_index + self.limit else from_index + self.limit
        print("Initialization of dataset memory from {} to {}".format(from_index, last_index))
        for index in range(from_index, last_index):
            if index % 100 == 0:
                print("Index processed: {}".format(index))
            if self.save_tensor:
                self.memory[str(index)] = self[index]
            else:    # store only pictures
                org_img, cls_img, _ = self.get_images_from_final_structure(index, to_tensor=False)
                self.memory[str(index)] = org_img, cls_img

        self.memory_init_on = False
        print("Dataset memory is fulfilled with {} elements from {} records, first index: {}, last index: {}"
              .format(len(self.memory), len(self.final_structure), from_index, last_index))

    def init_from_scratch(self, files):
        """
        Run it when cache manager does not have a cache hit
        :files: files from Couch class - see its structure in init
                triplets: (origin_image_path, classes_image_path, objects_image_path)
        """
        for i, triplet in enumerate(files):
            if i % 5 == 0:
                print("Cropping iteration {} ({} %), final_structure_size: {}, {}".format(
                                                i, str((i/len(files))*100),
                                                len(self.final_structure),
                                                self.final_structure[-1] if len(self.final_structure) > 0 else "")
                )
            origin_img, class_img, object_img = triplet
            self.cropper.set_imgs(origin_img, class_img, object_img)
            while not self.cropper.is_finished:
                actual_coords = self.cropper.coords[0]
                _, class_img_cv2, _ = self.cropper.next_crop()
                classes_in_image = get_classes_in_image(class_img_cv2)
                if len(classes_in_image) > 0:  # it has class so register it
                    self.final_structure.append((origin_img, class_img, object_img, actual_coords, 0))
                    self.cache.append_cache((origin_img, class_img, object_img, actual_coords, 0, "|".join(list(map(lambda c: "{},{},{}".format(c[0],c[1],c[2]), classes_in_image)))))

    def __len__(self):
        return len(self.final_structure)

    def get_images_from_final_structure(self, index, to_tensor=True):
        origin_img, class_img, object_img, coord, transformation = self.final_structure[index]  # get element from final_structure
        self.cropper.set_imgs(origin_img, class_img, object_img, transform=transformation, measure_coords=False)  # set the cropper image to analyze
        self.cropper.coords = [coord]  # override cropper coord by our one registered coord => only one image can be received
        return self.cropper.next_crop(convert_to_tensor=to_tensor)  # get this one crop

    def move_axis(self, input, target):
      """From [H, W, C] to [C, H, W]"""
      inp = np.moveaxis(input, -1, 0)
      tar = np.moveaxis(target, -1, 0)
      return inp, tar

    def do_transformation(self, origin_cv2, class_cv2):
        """
        :param origin_cv2: origin image in cv2 format
        :param class_cv2: class image in cv2 format
        """
        # https://www.kaggle.com/alexalex02/semantic-segmentation-of-aerial-images - edited :)
        # cv2.imshow("classes", class_cv2)
        # cv2.waitKey(0)
        origin_pil = cv2_to_pil(origin_cv2)  # origin image transfroms to PIL (RGB, opencv has BGR)
        class_pil = cv2_to_pil(class_cv2)  # the same with class image
        gt = np.array(class_pil)  # represent class image as np array
        trainId = self.mapper.transform(gt, dtype=np.uint8)  # transform the np array that contains elements with RGB values [R,G,B]
        # to a matrix with same shapes as class_pil image but instead of colors ([R,G,B]) we put the index of class (0 to 15 in our case)
        if self.cross_entropy:  # returns the matrix for cross entropy (with class index instead of pixel)
            x, y = self.move_axis(np.array(origin_pil) / 255.0, np.moveaxis(np.array(trainId), -1, 0))  # Upraveno FIX
            x, y = torch.from_numpy(x).type(self.inputs_dtype), torch.from_numpy(y).type(
                torch.LongTensor)  # Upraveno FIX
            return x, y

        if self.mapper.id_tab is not None:
            masks = [(trainId == i) for i, _ in enumerate(self.mapper.id_tab.values())]
            # for each class we create its mask (16 (number of classes) True/False matrices)
            mask = np.stack(masks, axis=-1).astype('float')
            # convert the matrices to matrices that each matrix contains line of the picture and for each pixel (column)
            # of the picture it contains list (array) of flags for class (this array has 16 elements - number of classes)
        else:
            mask = np.expand_dims(trainId, 2)

        # transfer to array
        origin_pil = np.array(origin_pil) / 255.0  # normalize
        mask = np.array(mask)
        origin_pil, mask = self.move_axis(origin_pil, mask)
        if self.to_tensor:
            # Typecasting
            x, y = torch.from_numpy(origin_pil).type(self.inputs_dtype), torch.from_numpy(mask).type(
                self.targets_dtype)
            return x, y
        return origin_pil, mask

    def __getitem__(self,
                    index: int):
        if str(index) in self.memory.keys():
            if self.save_tensor:
                return self.memory[str(index)]
            else:  # we have to transform picture to tensor
                org_img, cls_img = self.memory[str(index)]
                return self.do_transformation(org_img, cls_img)
        elif not self.memory_init_on and self.limit is not None:
            self.init_memory(from_index=index)

        origin_cv2, class_cv2, _ = self.get_images_from_final_structure(index, to_tensor=False)  # we have openCV images

        origin_pil, mask = self.do_transformation(origin_cv2, class_cv2)

        return origin_pil, mask

    def plot_histogram(self):
        self.cache.plot_histogram()

    def filter_dataset(self, classes_to_filter, only_these=False):
        new_couch = self.cache.filter_dataset(classes_to_filter, only_these=only_these)
        new_dataset = CropperSegmentationDataSet(width=self.cache.width, height=self.cache.height, couch=new_couch)
        return new_dataset

    def __sizeof__(self):
        sum = 0
        sum += get_size(self.final_structure)


class CacheManager:

    def __init__(self, width: int, height: int, couch_instance: Couch, cache_dir: str = "dataset_cache"):
        self.couch = couch_instance
        self.width = width
        self.height = height
        self.cache_dir = cache_dir
        if not os.path.exists(cache_dir):
            os.makedirs(cache_dir)

        self.cache_file_end_str = "{}x{}.csv".format(self.width, self.height)

        self.cache_file = None
        self.search_cache()

        """
        Cache dictionary stores key values
            key: origin_name
            value: final structure => (origin_img, class_img, object_img, (x, y), transformation)
        """
        self.cache_dictionary = {}
        self.mapper = ColorMapper()
        self.histogram = []
        self.init_cache()

    def init_cache(self):
        self.search_cache()

        def local_file(file):
            return os.path.join(self.couch.dataset, file)

        counter = 0
        if self.cache_file is not None:
            self.cache_dictionary = {}
            with open(self.cache_file) as opened_cache_file:
                for line in opened_cache_file:
                    origin_img_name, x, y, transformation, class_str = line.split(";", 4)
                    #origin_img, class_img, object_img = local_file(origin_img), local_file(class_img), local_file(object_img)
                    origin_img, class_img, object_img = self.couch.get_triplet(origin_img_name, self.couch.train + self.couch.val)
                    if origin_img is None:  # skip loading from cache if the Couch does not need it
                        continue

                    x, y, transformation = int(x), int(y), int(transformation)
                    if not get_origin_name(origin_img) in self.cache_dictionary.keys():
                        self.cache_dictionary[get_origin_name(origin_img)] = []
                    # add to dictionary
                    self.cache_dictionary[get_origin_name(origin_img)].append((origin_img, class_img, object_img, (x, y), transformation))
                    self.analyze_classes(class_str)
                    counter += 1
        print("Dictionary initialized (keys: {}: {}, size: {})".format(len(self.cache_dictionary), self.cache_dictionary.keys() if len(self.cache_dictionary) <= 5 else "", counter))

    def filter_dataset(self, classes_to_filter, only_these=False) -> Couch:
        """
        :param classes_to_filter: list of classes in format: [[R,G,B],[R2,G2,B2],...] => (classes which you wish you
            want to let in dataset), other images which don't contain one of these classes are removed from the list
        :param only_these: set True if you want to couch contains images which has only these classes and not other
            classes (only background plus these classes)
        :return: couch with filtered data
        """
        self.search_cache()
        couch_data = self.couch.train + self.couch.val
        filtered_origins = {}
        if self.cache_file is not None:
            def to_triplets(color_array=[]):
                return list(map(lambda c: self.mapper.clr2id(c), color_array))

            required_classes = to_triplets(classes_to_filter)

            with open(self.cache_file) as opened_cache_file:
                for line in opened_cache_file:
                    origin_img_name, x, y, transformation, class_str = line.split(";", 4)
                    origin_img, class_img, object_img = self.couch.get_triplet(origin_img_name, couch_data)
                    if origin_img is None:  # skip loading from cache if the Couch does not need it
                        continue
                    classes_in_image = self.analyze_classes(class_str, edit_histogram=False)
                    add_to_filtered = False
                    for _, class_in_image in enumerate(classes_in_image):
                        if only_these and class_in_image not in required_classes:
                            add_to_filtered = False
                            break
                        if not only_these and class_in_image in required_classes:
                            add_to_filtered = True
                            break
                    if add_to_filtered:
                        if origin_img_name not in filtered_origins.keys():
                            filtered_origins[origin_img_name] = add_to_filtered
                        elif only_these:
                            filtered_origins[origin_img_name] = filtered_origins[origin_img_name] and add_to_filtered
                        else:
                            filtered_origins[origin_img_name] = filtered_origins[origin_img_name] or add_to_filtered

        new_couch = Couch(self.couch.dataset)
        new_couch.test, new_couch.train, new_couch.val = [], [], []
        for origin_img_name, propagate in filtered_origins.items():
            if propagate:
                origin_image, class_img, object_img = self.couch.get_triplet(origin_img_name, couch_data)
                if "val" in origin_image:
                    new_couch.val.append(origin_image)
                    new_couch.val.append(class_img)
                    new_couch.val.append(object_img)
                else:
                    new_couch.train.append(origin_image)
                    new_couch.train.append(class_img)
                    new_couch.train.append(object_img)
        print("Filtering done: couch contains {} triplets ({} files)".format(len(filtered_origins), len(new_couch.train + new_couch.val)))
        return new_couch

    def filter_dataset_objects_in_the_middle(self, target_file, min_edge_padding=10):
        """
        Iterates over cache and stores only crops which contain object inside the crop which has defined padding
        :param target_file: target file name to store filtered data
        :param min_edge_padding: how far from the edge, should be at least one object in the crop (program creates
                                    bounding box around object and each point has to be inside the padding area)
        :return: couch with filtered data
        """
        self.search_cache()
        couch_data = self.couch.train + self.couch.val
        min_x, max_x, min_y, max_y = min_edge_padding, self.width - min_edge_padding, min_edge_padding, self.height - min_edge_padding
        if self.cache_file is not None:
            cropper = Cropper(width=self.width, height=self.height)
            iteration = 0

            def point_outside(p) -> bool:
                try:
                    x = p[0][0]
                    y = p[0][1]
                    return x < min_x or x > max_x or y < min_y or y > max_y
                except:
                    print("Exception: {}".format(p))
                    return True
            with open(self.cache_file) as opened_cache_file:
                for line in opened_cache_file:
                    origin_img_name, x, y, transformation, class_str = line.split(";", 4)
                    origin_img, class_img, object_img = self.couch.get_triplet(origin_img_name, couch_data)
                    iteration += 1
                    if origin_img is None:  # skip loading from cache if the Couch does not need it
                        continue
                    cropper.set_imgs(origin_img, class_img, object_img)
                    cropper.coords = [(int(x), int(y))]
                    o_i, c_i, ob_i = cropper.next_crop()

                    imgray = cv2.cvtColor(c_i, cv2.COLOR_BGR2GRAY)
                    ret, thresh = cv2.threshold(imgray, 10, 255, cv2.THRESH_BINARY)
                    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

                    f_contours = []
                    store = False
                    for _, current_contours in enumerate(contours):
                        if len(list(filter(lambda p: point_outside(p), current_contours))) <= 0:
                            #c_i = cv2.drawContours(c_i, current_contours, -1, (0, 0, 255), 3)
                            f_contours.append(current_contours)
                            store = True

                    if iteration % 1000 == 0:
                        c_i = cv2.rectangle(c_i, (min_x, min_y), (max_x, max_y), (0, 0, 255), 3)
                        c_i = cv2.drawContours(c_i, f_contours, -1, (0, 0, 255), 3)
                        result = cv2.hconcat([o_i, c_i, ob_i])
                        cv2.imshow("Result", result)
                        cv2.waitKey(0)

                    if store:
                        line = ";".join(
                            [origin_img_name, x, y, transformation, class_str])
                        if target_file is not None:
                            with open(target_file, 'a') as file:
                                file.write("{}\n".format(line))

        print("Dataset is filtered and located in {}.".format(target_file))

    def analyze_classes(self, class_str: str, edit_histogram=True):
        classes_in_the_picture = []
        if "|" in class_str:
            for i, class_colors in enumerate(class_str.split("|")):
                class_colors = class_colors.split(",")
                r, g, b = int(class_colors[0]), int(class_colors[1]), int(class_colors[2])
                classes_in_the_picture.append(self.mapper.clr2id([r,g,b]))
        else:
            class_colors = class_str.split(",")
            r, g, b = int(class_colors[0]), int(class_colors[1]), int(class_colors[2])
            classes_in_the_picture.append(self.mapper.clr2id([r,g,b]))

        if edit_histogram:
            for name, color in self.mapper.clr_tab.items():
                id_v = self.mapper.clr2id(color)
                if id_v in classes_in_the_picture:
                    self.histogram.append(name)
        return classes_in_the_picture

    def plot_histogram(self):
        labels, counts = np.unique(self.histogram, return_counts=True)

        ticks = range(len(counts))
        plt.bar(ticks, counts, align='center')
        plt.xticks(ticks, labels, rotation='vertical')
        plt.show()

    def list_caches(self, dir_to_search=None):
        if dir_to_search is None:
            dir_to_search = self.cache_dir

        cache_files = []
        for filename in os.listdir(dir_to_search):
            if filename.endswith(self.cache_file_end_str):
                cache_files.append(os.path.join(dir_to_search, filename))
            else:
                try:
                    cache_files = cache_files + self.list_caches(os.path.join(dir_to_search, filename))
                except:
                    print("{} is not folder".format(os.path.join(dir_to_search, filename)))
        return cache_files

    def search_cache(self):
        caches = self.list_caches()
        if len(caches) > 0:
            print("Cache hit ({}x): {}".format(len(caches), caches[0]))
            self.cache_file = caches[0]
        else:
            print("Cache MISS")
            self.cache_file = None

    def append_cache(self, record):
        """
        Appends cache file by record
            record format is: (origin_img, class_img, object_img, (x, y), transformation, classes)
        """
        origin_img, class_img, object_img, actual_coords, transformation, classes_in_image = record
        x, y = actual_coords

        def file(path: str):
            # return os.path.basename(os.path.normpath(path))
            return path.split(self.couch.dataset)[1]  # return the path in dataset directory
        line = ";".join([get_origin_name(origin_img), str(x), str(y), str(transformation), classes_in_image])
        if self.cache_file is not None:
            with open(self.cache_file, 'a') as file:
                file.write("{}\n".format(line))
        else:
            self.cache_file = os.path.join(self.cache_dir, "cache_" + self.cache_file_end_str)
            print("Cache manager Creates new cache file {}".format(self.cache_file))
            self.append_cache(record)

    def get_records_and_triplets(self):
        """
        Returns a tuple: satisfied_records_array, unsatisfied_triplets_array
            satisfied_record format is: (origin_img, class_img, object_img, (x, y), transformation)
            unsatisfied_triplet format is: (origin_img, class_img, object_img)
        """
        satisfied = []
        unsatisfied = []

        all_files = self.couch.train + self.couch.val
        for _, origin_name in enumerate(self.couch.get_iterable_origins(all_files)):
            if origin_name in self.cache_dictionary.keys():
                for _, record in enumerate(self.cache_dictionary[origin_name]):
                    satisfied.append(record)
            elif len(origin_name) > 0:
                origin_img, class_img, object_img = self.couch.get_triplet(origin_name, all_files)
                unsatisfied.append((origin_img, class_img, object_img))
                print("Unsatisfied: {} ({})".format(origin_name, origin_img))
        print("get_records_and_triplets: satisfied: {} records found, unsatisfied: {} files".format(len(satisfied), len(unsatisfied)))
        return satisfied, unsatisfied

from enum import Enum
class OB(Enum):
    nothing = 0,
    plane = 1,
    ship= 2,
    storage_tank = 3,
    baseball_diamond = 4,
    tennis_court = 5,
    basketball_court = 6,
    ground_track_field = 7,
    harbor = 8,
    bridge = 9,
    large_vehicle = 9,
    small_vehicle = 10,
    helicopter = 11,
    roundabout = 12,
    swimming_pool = 13,
    soccer_ball_field = 14


# https://www.kaggle.com/alexalex02/semantic-segmentation-of-aerial-images
class ColorMapper:
    def __init__(self):
    # color table.
        self.clr_tab = self.createColorTable()
    # id table.
        id_tab = {}
        for k, v in self.clr_tab.items():
            id_tab[k] = self.clr2id(v)
        self.id_tab = id_tab

    def createColorTable(self):
        clr_tab = {}
        clr_tab['Nothing'] = [0, 0, 0]
        clr_tab['Plane'] = [0, 127, 255]
        clr_tab['Ship'] = [0, 0, 63]
        clr_tab['Storage_tank'] = [0, 63, 63]
        clr_tab['Baseball_diamond'] = [0, 63, 0]
        clr_tab['Tennis_court'] = [0, 63, 127]
        clr_tab['Basketball_court'] = [0, 63, 191]
        clr_tab['Ground_track_field'] = [0, 63, 255]
        clr_tab['Harbor'] = [0, 100, 155]
        clr_tab['Bridge'] = [0, 127, 63]
        clr_tab['Large_vehicle'] = [0, 127, 127]
        clr_tab['Small_vehicle'] = [0, 0, 127]
        clr_tab['Helicopter'] = [0, 0, 191]
        clr_tab['Roundabout'] = [0, 191, 127]
        clr_tab['Swimming_pool'] = [0, 0, 255]
        clr_tab['Soccer_ball_field'] = [0, 127, 191]
        return clr_tab

    def colorTable(self):
        return self.clr_tab

    def clr2id(self, clr):
        return clr[0]+clr[1]*255+clr[2]*255*255

  #transform to uint8 integer label
    def transform(self,label, dtype=np.int32):
        height,width = label.shape[:2]
    # default value is index of clutter.
        newLabel = np.zeros((height, width), dtype=dtype)
        id_label = label.astype(np.int64)
        id_label = id_label[:,:,0]+id_label[:,:,1]*255+id_label[:,:,2]*255*255
        for tid,val in enumerate(self.id_tab.values()):
            mask = (id_label == val)
            newLabel[mask] = tid
        return newLabel

  #transform back to 3 channels uint8 label
    def inverse_transform(self, label):
        label_img = np.zeros(shape=(label.shape[0], label.shape[1],3),dtype=np.uint8)
        values = list(self.clr_tab.values())
        for tid,val in enumerate(values):
            mask = (label==tid)
            label_img[mask] = val
        return label_img


def pil_to_cv2(pil_img):
    # use numpy to convert the pil_image into a numpy array
    numpy_image = np.array(pil_img)

    # convert to a openCV2 image, notice the COLOR_RGB2BGR which means that
    # the color is converted from RGB to BGR format
    return cv2.cvtColor(numpy_image, cv2.COLOR_RGB2BGR)


def cv2_to_pil(opencv_image):
    # convert from openCV2 to PIL. Notice the COLOR_BGR2RGB which means that
    # the color is converted from BGR to RGB
    color_coverted = cv2.cvtColor(opencv_image, cv2.COLOR_BGR2RGB)
    return Image.fromarray(color_coverted)


def analyze_couch(couch: Couch):
    couch1, couch_rest = couch.split_couch(0.2)
    couch_r1, couch_r2 = couch_rest.split_couch(0.5)  # 40 % and 40 %
    couch2, couch3 = couch_r1.split_couch(0.5)  # 20 % and 20 %
    couch4, couch5 = couch_r2.split_couch(0.5)  # 20 % and 20 %
    for i, couch_tmp in enumerate([couch1, couch2, couch3, couch4, couch5]):
        half1, half2 = couch_tmp.split_couch(0.5)

        train, valid = half1.split_couch(0.8)
        dataset_train = CropperSegmentationDataSet(train, width=512, height=512)
        dataset_valid = CropperSegmentationDataSet(valid, width=512, height=512)
        cls_t = np.unique(dataset_train.cache.histogram)
        print("Train classes: {} = {}".format(len(cls_t), cls_t))
        cls_v = np.unique(dataset_valid.cache.histogram)
        print("Valid classes: {} = {}".format(len(cls_v), cls_v))
        print("Missing: {}".format(list(filter(lambda t: t not in cls_v, list(cls_t)))))
        dataset_train.plot_histogram()
        dataset_valid.plot_histogram()
        save = input("Save? [y/n]: ")
        if save == "y" or save == "Y":
            train_dir_to_save = input("Path to train dir:")
            val_dir_to_save = input("Path to val dir:")
            train.save_train_valid_data_to_dir(train_dir_to_save, delete=True)
            valid.save_train_valid_data_to_dir(val_dir_to_save, delete=True)


def get_size(obj, seen=None):
    """Recursively finds size of objects"""
    size = sys.getsizeof(obj)
    if seen is None:
        seen = set()
    obj_id = id(obj)
    if obj_id in seen:
        return 0
    # Important mark as seen *before* entering recursion to gracefully handle
    # self-referential objects
    seen.add(obj_id)
    if isinstance(obj, dict):
        size += sum([get_size(v, seen) for v in obj.values()])
        size += sum([get_size(k, seen) for k in obj.keys()])
    elif hasattr(obj, '__dict__'):
        size += get_size(obj.__dict__, seen)
    elif hasattr(obj, '__iter__') and not isinstance(obj, (str, bytes, bytearray)):
        size += sum([get_size(i, seen) for i in obj])
    return size

if __name__ == '__main__':
    couch = Couch(directory)
    # cache = CacheManager(width=512, height=512, couch_instance=couch)
    # cache.filter_dataset_objects_in_the_middle('dataset_cache/cache.csv')
    # cm = ColorMapper()
    # cls = [cm.clr_tab['Ship'], cm.clr_tab['Harbor'], cm.clr_tab['Large_vehicle'], cm.clr_tab['Small_vehicle'], cm.clr_tab['Plane']]
    # dataset_ex = CropperSegmentationDataSet(couch, width=512, height=512)
    # dataset_f = dataset_ex.filter_dataset(cls)
    # dataset_f.plot_histogram()
    # dataset_o_f = dataset_ex.filter_dataset(cls, only_these=True)
    # dataset_o_f.plot_histogram()
    # exit(0)
    couch1, couch2 = couch.split_couch(0.001)
    dataset = CropperSegmentationDataSet(couch1, width=512, height=512, limit=50, save_tensor=False)
    print("Dataset size: {} B".format(get_size(dataset.memory)))
    origin, classes = dataset[1]
    origin2, classes2 = dataset[49]
    print(classes[0])
    dataset.plot_histogram()
    dataset.to_tensor = True
    x, y = dataset[1]
    #print(origin)
    print(y)
    print(f'x = shape: {x.shape}; type: {x.dtype}')
    print(f'x = min: {x.min()}; max: {x.max()}')
    print(f'y = shape: {y.shape}; class: {y.unique()}; type: {y.dtype}')
    # cv2.imshow("Origin", origin)
    cv2.imshow("Classes", cv2.imread("iSAID_Devkit/preprocess/dataset/iSAID/val/images/P0003_instance_color_RGB.png"))
    cv2.waitKey(0)
