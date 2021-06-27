import os, shutil
import json, cv2, re, imutils
from skimage.metrics import structural_similarity
from cropper import Cropper
from PIL import Image
import numpy as np
from random import shuffle

directory = "../../data/"


def has_origin(name, origin_name):
    """
    Checks if the name contains the specific string
    :param name:
    :param origin_name:
    :return: True or False
    """
    return origin_name in name


def get_origin_name(name):
    """
    Returns the name of the picture without path
    :param name: name should contain P[0-9]{4}.png
    :return:
    """
    str_f = re.search(r"P[0-9]{4}.png", name)
    if str_f is not None:
        str_f = str_f.group(0)
    if str_f is None:
        return ""
    else:
        return re.search(r"P[0-9]{4}", str_f).group(0)


def get_imgs_difference_structural_similarity(reference, to_compare):
    """
    Returns the score of the structural similarity
    from: https://www.pyimagesearch.com/2017/06/19/image-difference-with-opencv-and-python/
    :param reference: reference image
    :param to_compare: generated image by network
    :return: score (<-1;1> and 1 is the perfect match), reference image, compared image, diff highlight, thresh
    """
    img1 = cv2.cvtColor(reference, cv2.COLOR_BGR2GRAY)
    img2 = cv2.cvtColor(to_compare, cv2.COLOR_BGR2GRAY)

    (score, diff) = structural_similarity(img1, img2, full=True)
    diff = (diff * 255).astype("uint8")

    thresh = cv2.threshold(diff, 0, 255,
                           cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]
    cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    # loop over the contours
    for c in cnts:
        # compute the bounding box of the contour and then draw the
        # bounding box on both input images to represent where the two
        # images differ
        (x, y, w, h) = cv2.boundingRect(c)
        cv2.rectangle(img1, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cv2.rectangle(img2, (x, y), (x + w, y + h), (0, 0, 255), 2)

    return score, img1, img2, diff, thresh


def get_imgs_difference_iou(reference, to_compare, is_PIL=False):
    """
    Returns the score of the intersection-over-union => union_area / (covered_area_in_both_pictures), if they are
    the same union_area = covered_area_in_both_pictures and result is 1 (perfect match)
    from: https://towardsdatascience.com/metrics-to-evaluate-your-semantic-segmentation-model-6bcb99639aa2
    :param reference: reference image
    :param to_compare: generated image by network
    :param is_PIL: set to True if given image is in pIL format
    :return: score (<0;1> and 1 is the perfect match)
    """
    #ref_f = "reference_tmp_img.png"
    #com_f = "to_compare_tmp_img.png"
    #cv2.imwrite(ref_f, reference)
    #cv2.imwrite(com_f, to_compare)
    if not is_PIL:
        reference = cv2.cvtColor(reference, cv2.COLOR_BGR2RGB)
        to_compare = cv2.cvtColor(to_compare, cv2.COLOR_BGR2RGB)

    ref_mask = np.array(reference).reshape(-1, 3)
    com_mask = np.array(to_compare).reshape(-1, 3)

    ref_c = 0
    com_c = 0
    both_c = 0

    #ref_cls = get_classes_in_image(reference)
    #com_cls = get_classes_in_image(to_compare)

    def not_background(triplet) -> bool:
        r, g, b = triplet
        return r != 0 or g != 0 or b != 0

    def same_triplets(triplet1, triplet2):
        r1, g1, b1 = triplet1
        r2, g2, b2 = triplet2
        return r1 == r2 and g1 == g2 and b1 == b2

    for i, ref_triplet in enumerate(ref_mask):
        com_triplet = com_mask[i]
        if not_background(ref_triplet) and not_background(com_triplet) and same_triplets(ref_triplet, com_triplet):
            both_c += 1
        else:
            if not_background(ref_triplet):
                ref_c += 1
            if not_background(com_triplet):
                com_c += 1

    #os.remove(ref_f)
    #os.remove(com_f)

    res_iou = both_c / (com_c + ref_c + both_c)
    print("both: {}, ref: {}, com: {}, res: {}".format(both_c, ref_c, com_c, res_iou))

    return res_iou


def get_classes_in_image(image):
    """
    Gets cv2 image and returns list of RGBs
    :param image: cv2 image
    :return: list of RGBs for each class
        [
            [R,G,B],
            [R,G,B],
            ...
        ]
    """
    # img_f = "img_tmp_img.png"
    # cv2.imwrite(img_f, image)
    # mask = Image.open(img_f)
    mask = Image.fromarray(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    mask = np.array(mask)
    #print(mask.reshape(-1, 3)[:5])
    mask = mask.reshape(-1,3)
    # get classes which are at img
    cls_ids = np.unique(mask, axis=0)

    # remove background
    cls_ids = cls_ids[1:]

    #os.remove(img_f)
    return cls_ids


def split_data(data, ratio, shuffle_flag):
    """
    Splits dataset into 2 chunks
    It returns at least 1 record for each result
    """
    if len(data) <= 1:
        return data[:], data[:]

    first_len = int(round(ratio * len(data)))
    copy_data = data[:]
    if shuffle_flag:
        shuffle(copy_data)

    # copy_data1 = [copy_data[0]] if len(copy_data[:first_len]) <= 1 else copy_data[:first_len]
    # copy_data2 = [copy_data[-1]] if len(copy_data[first_len:]) <= 1 else copy_data[first_len:]
    if first_len == 0:  # every dataset has at least 1 record but they don't have union
        first_len += 1
    elif first_len == len(copy_data):
        first_len -= 1
    return copy_data[:first_len], copy_data[first_len:]


def edge_detection_from_class_img(class_img):
    """
    Detects edges in the class image
    :param class_img: cv2 image to convert
    :return: cv2 black image with white lines
    """
    imgray = cv2.cvtColor(class_img, cv2.COLOR_BGR2GRAY)
    ret, thresh = cv2.threshold(imgray, 5, 255, cv2.THRESH_BINARY)
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

    background = np.zeros(class_img.shape, np.uint8)
    return cv2.drawContours(background, contours, -1, (255, 255, 255), 1)


def edge_detection_from_origin_img(origin_img, border=85):
    """
    Detects edges in the origin image
    :param origin_img: cv2 image to convert
    :param border: limit for canny
    :return: cv2 black image with white lines
    """
    gray = cv2.cvtColor(origin_img, cv2.COLOR_BGR2GRAY)
    return cv2.cvtColor(cv2.Canny(gray, border, 255), cv2.COLOR_GRAY2BGR)


class Couch:
    def __init__(self, dataset):
        self.dataset = dataset
        self.train = []
        self.val = []
        self.test = []
        self.init_subsets()

    def init_subsets(self):
        """
        Reset sets of datas (train, val, test)
        :return:
        """
        self.train, self.val, self.test = [], [], []
        data_to_iterate = self.list_dir()
        data_to_iterate.sort()
        for index, dato in enumerate(data_to_iterate):
            if "test" in dato:
                self.test.append(dato)
            elif "val" in dato:
                self.val.append(dato)
            elif "train" in dato:
                self.train.append(dato)
            else:
                self.train.append(dato)  # push it to the train if it does not have any label

        self.train.sort()
        self.test.sort()
        self.val.sort()

    def list_dir(self, dir_to_search=None):
        if dir_to_search is None:
            dir_to_search = self.dataset

        local_dataset = []
        for filename in os.listdir(dir_to_search):
            if filename.endswith(".png"):
                local_dataset.append(os.path.join(dir_to_search, filename))
            else:
                try:
                    local_dataset = local_dataset + self.list_dir(dir_to_search + filename + "/")
                except:
                    try:
                        local_dataset = local_dataset + self.list_dir(os.path.join(dir_to_search, filename))
                    except:
                        print("{} is not folder".format(dir_to_search + filename + "/"))
        return local_dataset

    def get_iterable_evals(self):
        """
        Returns pure list of file names for which we can iterate and get the files by get_val_triple
        :return:
        """
        set_to_ret = set({})
        for i, val in enumerate(self.val):
            set_to_ret.add(get_origin_name(val))

        return list(set_to_ret)

    def get_val_triple(self, origin_name):
        """
        Returns the triplet of images for origin_name
        :param origin_name:
        :return: triplet where the pictures are: origin_image_path, classes_image_path (each class has its own color), objects_image_path (each object has its own color)
        """
        for i, val in enumerate(self.val):
            if has_origin(val, origin_name):
                filtered_files = list(filter(lambda name: has_origin(name, origin_name), self.val))

                if len(filtered_files) >= 3:
                    return filtered_files[0], filtered_files[1], filtered_files[2]
                elif len(filtered_files) >= 1:
                    return filtered_files[0], filtered_files[0], filtered_files[0]

    def get_iterable_trains(self):
        """
        Returns pure list of file names for which we can iterate and get the files by get_train_triple
        :return:
        """
        set_to_ret = set({})
        for i, val in enumerate(self.train):
            set_to_ret.add(get_origin_name(val))

        return list(set_to_ret)

    def get_train_triple(self, origin_name):
        """
        Returns the triplet of images for origin_name
        :param origin_name:
        :return: triplet where the pictures are: origin_image_path, classes_image_path (each class has its own color), objects_image_path (each object has its own color)
        """
        for i, val in enumerate(self.train):
            if has_origin(val, origin_name):
                filtered_files = list(filter(lambda name: has_origin(name, origin_name), self.train))

                if len(filtered_files) >= 3:
                    return filtered_files[0], filtered_files[1], filtered_files[2]
                elif len(filtered_files) >= 1:
                    return filtered_files[0], filtered_files[0], filtered_files[0]

    def get_iterable_origins(self, data):
        """
        Returns pure list of file names for which we can iterate and get the files by get_triple
        :return:
        """
        set_to_ret = set({})
        for i, val in enumerate(data):
            set_to_ret.add(get_origin_name(val))

        return list(set_to_ret)

    def get_triplet(self, origin_name, data):
        """
        Returns the triplet of images for origin_name
        :param origin_name: origin name
        :param data: structure which should be discovered
        :return: triplet where the pictures are: origin_image_path, classes_image_path (each class has its own color), objects_image_path (each object has its own color)
        """
        for i, val in enumerate(data):
            if has_origin(val, origin_name):
                filtered_files = list(filter(lambda name: has_origin(name, origin_name), data))

                if len(filtered_files) >= 3:
                    return filtered_files[0], filtered_files[1], filtered_files[2]
                elif len(filtered_files) >= 1:
                    return filtered_files[0], filtered_files[0], filtered_files[0]
        return None, None, None

    def get_triplets_with_class_template(self, data):
        """
        This is a list of records (triplet: origin_image_path, classes_image_path, objects_image_path).
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
        files = []
        for _, origin in enumerate(self.get_iterable_origins(data)):
            origin_img, class_img, object_img = self.get_triplet(origin, data)
            if origin_img == class_img:
                continue
            else:
                files.append((origin_img, class_img, object_img))  # append triplet because it contains class template

        return files

    def triplets_to_array(self, triplets):
        """
        Unpack triplets into 1D array
        :param triplets: list of records (origin_image_path, classes_image_path, objects_image_path)
        :return : array of strings
        """
        data = []
        for _, triplet in enumerate(triplets):
            origin_image_path, classes_image_path, objects_image_path = triplet
            data.append(origin_image_path)
            data.append(classes_image_path)
            data.append(objects_image_path)

        return data

    def split_couch(self, ratio, shuffle_flag=False):
        """
        Splits couch into 2 couches with different datasets
        For train and val parts it packed them into triplets (when shuffling is ON it would throw many class
        templates away), after splitting, they are unpacked
        Files without
        :ratio: <0,1.0> ratio of datasets length
        :return: Couch, Couch => Couch1 and Couch2 (with its datasets)
        """
        test1, test2 = split_data(self.test, ratio, shuffle_flag)
        train1, train2 = split_data(self.get_triplets_with_class_template(self.train), ratio, shuffle_flag)
        val1, val2 = split_data(self.get_triplets_with_class_template(self.val), ratio, shuffle_flag)

        dataset1 = Couch(self.dataset)
        dataset2 = Couch(self.dataset)

        dataset1.test = test1
        dataset1.train = self.triplets_to_array(train1)
        dataset1.val = self.triplets_to_array(val1)

        dataset2.test = test2
        dataset2.train = self.triplets_to_array(train2)
        dataset2.val = self.triplets_to_array(val2)

        return dataset1, dataset2

    def save_train_valid_data_to_dir(self, target_directory, delete=False):
        """
        Saves train/valid data to directory (it should be only triplets)
        """
        if not os.path.exists(target_directory):
            os.makedirs(target_directory)
        elif delete:
            shutil.rmtree(target_directory)
            os.makedirs(target_directory)

        for _, file in enumerate(self.train + self.val):
            base_name = os.path.basename(file)
            file_target_path = os.path.join(target_directory, base_name)
            shutil.copyfile(file, file_target_path)
        print("Files ({}) copied to {}".format(len(self.test + self.train + self.val), target_directory))

    def filter_dataset(self, classes, only_these=False):
        """
        :param classes: list of classes in format: [[R,G,B],[R2,G2,B2],...] => (classes which you wish you want to let
            in dataset), other images which don't contain one of these classes are removed from the list
        :param only_these: set True if you want to couch contains images which has only these classes and not other
            classes (only background plus these classes)
        :return: couch with filtered data
        """
        data = self.train + self.val
        filtered_triplets = []
        def to_triplets(color_array=[]):
            return list(map(lambda c: (c[0],c[1],c[2]), color_array))

        required_classes = to_triplets(classes)
        for i, triplet in enumerate(self.get_triplets_with_class_template(data)):
            origin_image, class_img, object_img = triplet
            classes_in_image = to_triplets(get_classes_in_image(cv2.imread(class_img)))
            add_to_filtered = False
            for _, class_in_image in enumerate(classes_in_image):
                if only_these and class_in_image not in required_classes:
                    add_to_filtered = False
                    break
                if not only_these and class_in_image in required_classes:
                    add_to_filtered = True
                    break
            if add_to_filtered:
                filtered_triplets.append(triplet)

        new_couch = Couch(self.dataset)
        new_couch.test, new_couch.train, new_couch.val = [], [], []
        for _, triplet in enumerate(filtered_triplets):
            origin_image, class_img, object_img = triplet
            if "val" in origin_image:
                new_couch.val.append(origin_image)
                new_couch.val.append(class_img)
                new_couch.val.append(object_img)
            else:
                new_couch.train.append(origin_image)
                new_couch.train.append(class_img)
                new_couch.train.append(object_img)
        print("Filtering done: couch contains {} triplets ({} files)".format(len(filtered_triplets), len(new_couch.train + new_couch.val)))
        return new_couch



if __name__ == '__main__':
    couch = Couch(directory)
    c1, c2 = couch.split_couch(0.2)
    print(couch.val)
    print(c1.val)
    print(c2.val)
    s_example, s_reference, s_to_compare = couch.get_train_triple(couch.get_iterable_trains()[450])
    print(s_example, s_reference, s_to_compare)
    cropper = Cropper()
    cropper.set_imgs(s_example, s_reference, s_to_compare)
    example, reference, reference_boundary = cropper.next_crop()
    edge = edge_detection_from_origin_img(example)
    alpha = 0.8
    wedge = cv2.addWeighted(example, alpha, edge, 1.0-alpha, 0.0)
    wedge = cv2.bitwise_or(example,edge)
    cv2.imshow("Edges", cv2.hconcat([example, edge_detection_from_class_img(reference), edge, wedge]))
    print("{}".format(get_classes_in_image(reference)))

    # result = model.runComputation(example)
    result = reference

    num_rows, num_cols = reference.shape[:2]
    translation_matrix = np.float32([[1, 0, 1], [0, 1, 0]])
    img_translation = cv2.warpAffine(reference, translation_matrix, (num_cols, num_rows))

    score, ref, res, diff, thresh = get_imgs_difference_structural_similarity(reference, img_translation)
    # cv2.imshow("Example", example)
    # cv2.imshow("Reference", ref)
    # cv2.imshow("Modified", res)
    # cv2.imshow("Diff", diff)
    # cv2.imshow("Thresh", thresh)
    print("Score get_imgs_difference_structural_similarity: {}".format(score))
    #im_h = cv2.hconcat([example, reference, reference_boundary])
    #cv2.imshow("All", im_h)
    #im_h = cv2.hconcat([thresh, diff])
    #cv2.imshow("Thresh and diff", im_h)
    #print(get_classes_in_image(reference))

    im_h = cv2.hconcat([reference, img_translation])
    cv2.imshow("Translation", im_h)
    print("Score get_imgs_difference_iou: {}".format(get_imgs_difference_iou(reference, img_translation)))
    cv2.waitKey(0)

    # for i, img in enumerate(os.listdir("results")):
    #     if "_target.png" in img:
    #         generated = img.split("_target.png")[0]
    #         cv_gen = cv2.imread(os.path.join("results", generated + ".png"))
    #         cv_tar = cv2.imread(os.path.join("results", img))
    #         print("Score get_imgs_difference_iou: {}".format(get_imgs_difference_iou(cv_gen, cv_tar)))
    #         score, ref, res, diff, thresh = get_imgs_difference_structural_similarity(reference, img_translation)
    #         print("Score get_imgs_difference_structural_similarity: {}".format(score))

