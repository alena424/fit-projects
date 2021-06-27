from datetime import datetime
import os


class ExperimentLogger:
    def __init__(self, model_name, settings: dict, predictor, cross_entropy=False, experiment_dir=None):
        self.now = datetime.now()
        self.model_name = model_name

        gdrive = os.path.join(os.sep, "content", "drive", "My Drive")
        gdrive_dir = os.path.join(gdrive, "experiments", model_name + "-" + self.str_date())
        local_dir = os.path.join("experiments", model_name + "-" + self.str_date())
        print(gdrive, local_dir)

        if experiment_dir is None:
            if os.path.exists(gdrive):
                os.makedirs(gdrive_dir)
                self.experiments_dir = os.path.join(gdrive, "experiments")
                self.experiment_dir = gdrive_dir
            else:
                os.makedirs(local_dir)
                self.experiments_dir = os.path.join("experiments", "")
                self.experiment_dir = local_dir
        else:
            if not os.path.exists(experiment_dir):
                os.makedirs(experiment_dir)
            self.experiments_dir = os.path.join(experiment_dir.split("experiments")[0], "experiments", "")
            self.experiment_dir = experiment_dir

        print("Experiment is logged in {}".format(self.experiment_dir))

        self.epoch_images = os.path.join(self.experiment_dir, "epoch_images", "")
        self.eval_images = os.path.join(self.experiment_dir, "eval_images", "")
        self.loss_plots = os.path.join(self.experiment_dir, "loss_plots", "")
        self.models = os.path.join(self.experiment_dir, "models", "")

        self.loss_raw = os.path.join(self.loss_plots, "raw_epochs.txt")

        self.model_path = os.path.join(self.models, model_name + ".pkl")

        for dir_to_create in [self.epoch_images, self.eval_images, self.loss_plots, self.models]:
            if not os.path.exists(dir_to_create):
                os.makedirs(dir_to_create)

        self.file = os.path.join(self.experiment_dir, "experiment.txt")

        self.write(self.str_dict(settings))
        print(self.str_dict(settings))

        self.predictor = predictor
        self.cross_entropy = cross_entropy

    def str_date(self):
        return self.now.strftime("%d-%m-%Y_%H:%M:%S")

    def str_dict(self, dictionary: dict):
        str_dict = "=================\nSettings {}\n=================\n".format(self.model_name)
        for key, value in dictionary.items():
            str_dict += str(key) + ": " + str(value) + "\n"

        return str_dict + "=================\n"

    def loss_plot_path(self, epoch):
        return os.path.join(self.loss_plots, "dual_loss_epoch_{}.png".format(epoch))

    def write(self, line):
        with open(self.file, 'a') as file:
            file.write("{}\n".format(line))

    def write_raw_loss_epoch(self, epoch_num, data, data_val):
        avg = sum(data) / len(data)
        avg_val = sum(data_val) / len(data_val)
        with open(self.loss_raw, 'a') as file:
            file.write("Epoch{};{};{};{};{}\n".format(epoch_num, avg, avg_val, data, data_val))

    def list_dir(self, dir_to_search, file_extension):
        if dir_to_search is None:
            dir_to_search = self.dataset

        finds_elms = []
        for filename in os.listdir(dir_to_search):
            if filename.endswith(file_extension):
                finds_elms.append(os.path.join(dir_to_search, filename))
            else:
                try:
                    finds_elms = finds_elms + self.list_dir(dir_to_search + filename + "/", file_extension)
                except:
                    try:
                        finds_elms = finds_elms + self.list_dir(os.path.join(dir_to_search, filename), file_extension)
                    except:
                        # print("{} is not folder".format(dir_to_search + filename + "/"))
                        pass
        return finds_elms

    def benchmark_models(self, dataloader_128, dataloader_512, period_to_save=10, ignore_models=[]):
        def get_model_param(base_path, param_name):
            file = os.path.join(base_path, "experiment.txt")
            with open(file) as opened_file:
                for line in opened_file:
                    line = line.lower()
                    # print(line)
                    if ":" in line and param_name.lower() in line:
                        return line.split(":")[1].strip()
            return None

        def copy_model_param(base_path, target_file):
            file = os.path.join(base_path, "experiment.txt")
            with open(file) as opened_file:
                for line in opened_file:
                    if ":" in line and (len(line) < 50 or "description" in line.lower()):
                        target_file.write(line)
            return None

        models_to_iterate = self.list_dir(dir_to_search=self.experiments_dir, file_extension='pkl')
        priority = []
        for m in models_to_iterate:
            if 'good' in m or 'alca' in m:
                priority.append(m)
                models_to_iterate.remove(m)
        for model_path in priority + models_to_iterate:
            print("Benchmark for: {}".format(model_path))
            if model_path in ignore_models:
                print("Skipping")
                continue
            base_path = model_path.split("models")[0]
            model_name = model_path.split("models/")[1]
            p_width = get_model_param(base_path, "width")
            p_device = get_model_param(base_path, "device")
            p_entropy = get_model_param(base_path, "Cross entropy") == 'true'

            if p_width is not None:
                if p_width == "512":
                    selected_dataloader = dataloader_512
                else:
                    selected_dataloader = dataloader_128
                if p_device.strip() != str(device).strip():
                    print("Not the same device, actual: {} but required: {}".format(device, p_device))
                    continue
            else:
                print("Model ({}) does not have configurated size of pictures".format(
                    os.path.join(base_path, "experiment.txt")))
                continue

            if p_entropy != selected_dataloader.dataset.cross_entropy:
                selected_dataloader.dataset.cross_entropy = p_entropy
                selected_dataloader.dataset.init_memory(from_index=0)

            self.predictor.testing_DataLoader = selected_dataloader
            self.predictor.MAX_STEPS = len(selected_dataloader.dataset)
            model = torch.load(model_path)
            model.eval()
            counter = 1
            while os.path.exists(os.path.join(self.eval_images, "{}-{}".format(model_name, counter))):
                counter += 1
            images_to = os.path.join(self.eval_images, "{}-{}".format(model_name, counter), "")
            os.makedirs(images_to)
            results = os.path.join(images_to, "results.txt")
            with open(results, 'a') as file:
                file.write("Model {}\n".format(model_name))
                file.write("\tAt {}\n".format(model_path))
                copy_model_param(base_path, file)
            self.predictor.all_predict(
                cross_entropy=p_entropy,
                model=model,
                img_path=images_to,
                img_prefix="",
                result_file=results,
                period_save=period_to_save
            )

if __name__ == '__main__':
    e = ExperimentLogger("Example", {"Batch": 2, "Learning rate": 0.001}, predictor=None)
