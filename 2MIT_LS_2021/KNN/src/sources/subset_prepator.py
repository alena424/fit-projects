import os
import random
import cv2
from couch import Couch, get_origin_name
from cropper import Cropper


class SubsetCreator(Couch):
    def __init__(self, dataset, width=512, height=512, pictures=10, generate_classes=True, generate_objects=True):
        """
        Runs everything needed for data preparation
        :param dataset: path to dataset
        :param width: expected width of the images in subset
        :param height: expected height of the images in subset
        :param pictures: how many pictures should be selected for cropping operations
        :param generate_classes: default TRUE, if True, it creates dir with classes images and dir with joined images (origin vertically joined with classes_image)
        :param generate_objects: default TRUE, if True, it creates dir with objects images and dir with joined images (origin vertically joined with classes_image)
        """
        super().__init__(dataset)

        cropper = Cropper(width=width, height=height)

        dir_name = "tmp-data-{}x{}-from-{}-pictures".format(width, height, pictures)
        origins = os.path.join(dir_name, "origins")
        classes = os.path.join(dir_name, "classes")
        origins_classes_v_join = os.path.join(dir_name, "origin-classes-v-join")
        objects = os.path.join(dir_name, "objects")
        origins_objects_v_join = os.path.join(dir_name, "origin-objects-v-join")

        if not os.path.exists(origins):
            os.makedirs(origins)

        trains = self.get_iterable_trains()
        vals = self.get_iterable_evals()

        selection_set = []
        for _, val in enumerate(trains):
            origin, class_v, object_v = self.get_train_triple(val)
            selection_set.append((origin, class_v, object_v))
        for _, val in enumerate(vals):
            origin, class_v, object_v = self.get_val_triple(val)
            selection_set.append((origin, class_v, object_v))

        final_set = random.sample(selection_set, pictures)

        if generate_classes:
            if not os.path.exists(classes):
                os.makedirs(classes)
            if not os.path.exists(origins_classes_v_join):
                os.makedirs(origins_classes_v_join)

        if generate_objects:
            if not os.path.exists(objects):
                os.makedirs(objects)
            if not os.path.exists(origins_objects_v_join):
                os.makedirs(origins_objects_v_join)

        for _, (origin, class_v, object_v) in enumerate(final_set):
            print("Processing {}, {}, {}".format(origin, class_v, object_v))
            cropper.set_imgs(origin, class_v, object_v, add_randomly=5)
            counter = 1
            while not cropper.is_finished:
                origin_i, class_i, object_i = cropper.next_crop()
                # Check that classes are involved
                finded = False
                for l in class_i:
                    for pix in l:
                        for c in pix:
                            if c != 0:
                                finded = True
                                break
                        if finded:
                            break
                    if finded:
                        break
                if not finded:
                    continue
                path = "{}-{}.png".format(get_origin_name(origin), counter)
                # print("Writing: {}".format(os.path.join(origins, path)))
                cv2.imwrite(os.path.join(origins, path), origin_i)
                if generate_classes:
                    cv2.imwrite(os.path.join(classes, path), class_i)
                    cv2.imwrite(os.path.join(origins_classes_v_join, path), cv2.hconcat([origin_i, class_i]))
                if generate_objects:
                    cv2.imwrite(os.path.join(objects, path), object_i)
                    cv2.imwrite(os.path.join(origins_objects_v_join, path), cv2.hconcat([origin_i, object_i]))
                counter += 1

        print("Generating of {}-pictures-subset done. You find it in: {}".format(pictures, dir_name))


if __name__ == '__main__':
    SubsetCreator(dataset="data", width=512, height=512, pictures=1, generate_classes=True, generate_objects=True)