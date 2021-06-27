import numpy as np
import cv2, random, imutils
from torchvision import transforms
from PIL import Image

# https://www.programmersought.com/article/4469534250/
cv2_to_tensor = transforms.ToTensor()  # convert cv2 to tensor from (H, W, 3) to (C, H, W)

dataset = "../../data/"

width = 512
height = 512


class Cropper:

    def __init__(self, width=512, height=512):
        self.height = height
        self.width = width

        self.img1 = None
        self.img2 = None
        self.img3 = None
        self.is_finished = False

        self.image_path = ""

        self.coords = []

    def reset(self):
        self.img1 = None
        self.img2 = None
        self.img3 = None
        self.is_finished = False
        self.coords = []

    def set_imgs(self, img1, img2=None, img3=None, add_randomly=0, transform=0, measure_coords=True):
        """
        Set images for cropping, dataset contains for training and evaluation 3 pictures for one case (original,
        semantic and border mask). This method sets images to cut...
        If the only one image is set img2 and img3 are set for the same picture (the test set contains the only one
        picture for case without validation)
        If all three pictures are set up, they will be processed and cropped together.
        This methods also sets the coords where the pictures should be cropped. It is systemically done.
        There is an opportunity to set up an amount of randomly selected cropped picture for increase the number of
        samples.
        :param img1:
        :param img2:
        :param img3:
        :param add_randomly:
        :param transform: angle for rotation of the each image
        :param measure_coords: set true if the cropper should count the coords
        :return:
        """
        if self.image_path != img1:  # if the set image is the same as teh previous, skip it
            self.reset()
            if img2 is None or img3 is None:
                self.img1, self.img2, self.img3 = cv2.imread(img1), cv2.imread(img1), cv2.imread(img1)
            else:
                self.img1, self.img2, self.img3 = cv2.imread(img1), cv2.imread(img2), cv2.imread(img3)
        else:
            self.coords = []
            self.is_finished = False

        self.image_path = img1

        if transform != 0:
            self.img1 = imutils.rotate(self.img1, angle=transform)
            self.img2 = imutils.rotate(self.img2, angle=transform)
            self.img3 = imutils.rotate(self.img3, angle=transform)

        if measure_coords:
            x_interval = np.arange(start=0, stop=self.img1.shape[1], step=self.width)
            y_interval = np.arange(start=0, stop=self.img1.shape[0], step=self.height)
            for idx, val_x in enumerate(x_interval):
                for idx2, val_y in enumerate(y_interval):
                    self.coords.append((val_x, val_y))

            for _ in np.arange(start=0, stop=add_randomly):
                self.coords.append((
                    random.randrange(0, self.img1.shape[1] - self.width),
                    random.randrange(0, self.img1.shape[0] - self.height)
                ))

    def number_of_crops(self):
        """
        Returns number of crops per set image
        """
        return len(self.coords)

    def next_crop(self, convert_to_tensor=False):
        """
        Returns another triplet of crops from selected image. If the only one picture was set, each triplet member is
        from the same picture.
        <h3>How to iterate pictures?</h3>
        cropper.set_imgs("Path1", "Path2", "Path3", add_randomly=5)
            while not cropper.is_finished:
            img1, img2, img3 = cropper.next_crop()
            cv2.imshow('Image', img1)
            cv2.waitKey(0)
        :convert_to_tensor: if it set to True, returns triplet of tensors
        :return: triplet of images: cv2.Image, cv2.Image, cv2.Image
                    triplet where the pictures are: origin_image, classes_image (each class has its own color), objects_image (each object has its own color)
        """
        x, y = self.coords.pop(0)

        if len(self.coords) <= 0:
            self.is_finished = True

        origin_image = self.crop_img(self.img1, x, y, w=self.width, h=self.height)
        classes_image = self.crop_img(self.img2, x, y, w=self.width, h=self.height)
        objects_image = self.crop_img(self.img3, x, y, w=self.width, h=self.height)
        if convert_to_tensor:
            return cv2_to_tensor(origin_image), cv2_to_tensor(classes_image), cv2_to_tensor(objects_image)
        else:
            return origin_image, classes_image, objects_image

    def crop_img(self, img, t_l_x, t_l_y, w=512, h=512, show=False):
        """
        Crops the image
        :param img:
        :param t_l_x: left-top-x-coord conner of the crop
        :param t_l_y: left-top-y-coord conner of the crop
        :param w: width
        :param h: height
        :param show: if the picture should be displayed on the screen and it waits for user interaction
        (enter press, default: False)
        :return: cropped image
        """
        if t_l_x < 0:
            t_l_x = 0
        if t_l_y < 0:
            t_l_y = 0
        if t_l_x + w > img.shape[1]:
            t_l_x = img.shape[1] - w
        if t_l_y + h > img.shape[0]:
            t_l_y = img.shape[0] - h

        crop = img[t_l_y:t_l_y + h, t_l_x:t_l_x + w, :]

        if show:
            cv2.imshow('Image', crop)
            cv2.waitKey(0)

        return crop


if __name__ == '__main__':
    cropper = Cropper()
    cropper.set_imgs("P0006.png", add_randomly=5)
    while not cropper.is_finished:
        img1, img2, img3 = cropper.next_crop()
        cv2.imshow('Image', img1)
        cv2.waitKey(0)
