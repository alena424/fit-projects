import requests, os, shutil, zipfile
# from https://stackoverflow.com/questions/38511444/python-download-files-from-google-drive-using-url


def download_file_from_google_drive(id, destination):
    URL = "https://docs.google.com/uc?export=download"

    session = requests.Session()

    response = session.get(URL, params={'id': id}, stream=True)
    token = get_confirm_token(response)

    if token:
        params = {'id': id, 'confirm': token}
        response = session.get(URL, params=params, stream=True)

    save_response_content(response, destination)


def get_confirm_token(response):
    for key, value in response.cookies.items():
        if key.startswith('download_warning'):
            return value

    return None


def save_response_content(response, destination):
    CHUNK_SIZE = 32768

    with open(destination, "wb") as f:
        for chunk in response.iter_content(CHUNK_SIZE):
            if chunk:  # filter out keep-alive new chunks
                f.write(chunk)


if __name__ == "__main__":
    # file_id = 'TAKE ID FROM SHAREABLE LINK'
    # destination = 'DESTINATION FILE ON YOUR DISK'
    # download_file_from_google_drive(file_id, destination)

    files = [
        ("1zb_kEOXTtpsMuWAtuTtMIPLVUSpjGPPw", "dota_v1_train.zip"),
        ("1uCCCFhFQOJLfjBpcL5MC0DHJ9lgOaXWP", "dota_v1_val.zip"),
        ("12XhSgEt_Xw4FJQxLJZgMutw3awoAq2Ve", "isaid_instance_mask_train.zip"),
        ("1YLjZ1cmA9PH3OfzMF-eq6T-O9FTGvSrx", "isaid_semantic_mask_train.zip"),
        ("1GCExuFqEKOY5Hyp1WSAmdW6I4RAaxBGG", "isaid_instance_mask_val.zip"),
        ("1_PJy7kDVnp9tjETUbvwNjkLGpzDfqYh6", "isaid_semantic_mask_val.zip"),
        ("1fwiTNqRRen09E-O9VSpcMV2e6_d4GGVK", "isaid_test_1.zip"),
        ("1wTwmxvPVujh1I6mCMreoKURxCUI8f-qv", "isaid_test_2.zip")
    ]

    for i, data in enumerate(files):
        source_id, destination = data
        print("Downloading to: {} ({})".format(destination, source_id))
        download_file_from_google_drive(source_id, destination)

    test_dir = os.path.join(os.path.join('data', 'test'), 'images')
    test_zips = ["isaid_test_1.zip", "isaid_test_2.zip"]

    train_dir = os.path.join(os.path.join('data', 'train'), 'images')
    train_zips = ["dota_v1_train.zip", "isaid_instance_mask_train.zip", "isaid_semantic_mask_train.zip"]

    val_dir = os.path.join(os.path.join('data', 'val'), 'images')
    val_zips = ["dota_v1_val.zip", "isaid_instance_mask_val.zip", "isaid_semantic_mask_val.zip"]

    if not os.path.exists(test_dir):
        os.makedirs(test_dir)
    for zip_source in test_zips:
        with zipfile.ZipFile(zip_source) as zip_file:
            for member in zip_file.namelist():
                filename = os.path.basename(member)
                # skip directories
                if not filename:
                    continue

                # copy file (taken from zipfile's extract)
                source = zip_file.open(member)
                target = open(os.path.join(test_dir, filename), "wb")
                with source, target:
                    shutil.copyfileobj(source, target)

    if not os.path.exists(val_dir):
        os.makedirs(val_dir)
    for zip_source in val_zips:
        with zipfile.ZipFile(zip_source) as zip_file:
            for member in zip_file.namelist():
                filename = os.path.basename(member)
                # skip directories
                if not filename:
                    continue

                # copy file (taken from zipfile's extract)
                source = zip_file.open(member)
                target = open(os.path.join(val_dir, filename), "wb")
                with source, target:
                    shutil.copyfileobj(source, target)

    if not os.path.exists(train_dir):
        os.makedirs(train_dir)
    for zip_source in train_zips:
        with zipfile.ZipFile(zip_source) as zip_file:
            for member in zip_file.namelist():
                filename = os.path.basename(member)
                # skip directories
                if not filename:
                    continue

                # copy file (taken from zipfile's extract)
                source = zip_file.open(member)
                target = open(os.path.join(train_dir, filename), "wb")
                with source, target:
                    shutil.copyfileobj(source, target)
