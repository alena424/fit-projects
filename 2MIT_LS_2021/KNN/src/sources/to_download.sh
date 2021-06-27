
echo "Train data DOTA-v1 downloading...."
# https://drive.google.com/file/d/1zb_kEOXTtpsMuWAtuTtMIPLVUSpjGPPw/view?usp=sharing 
#    1zb_kEOXTtpsMuWAtuTtMIPLVUSpjGPPw
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1zb_kEOXTtpsMuWAtuTtMIPLVUSpjGPPw' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1zb_kEOXTtpsMuWAtuTtMIPLVUSpjGPPw" -O dota_v1_train.zip && rm -rf /tmp/cookies.txt



echo "Val data DOTA-v1 downloading...."
# https://drive.google.com/file/d/1uCCCFhFQOJLfjBpcL5MC0DHJ9lgOaXWP/view?usp=sharing
#    1uCCCFhFQOJLfjBpcL5MC0DHJ9lgOaXWP
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1uCCCFhFQOJLfjBpcL5MC0DHJ9lgOaXWP' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1uCCCFhFQOJLfjBpcL5MC0DHJ9lgOaXWP" -O dota_v1_val.zip && rm -rf /tmp/cookies.txt



echo "train data iSAID downloading...."
# https://drive.google.com/file/d/12XhSgEt_Xw4FJQxLJZgMutw3awoAq2Ve/view?usp=sharing
#    12XhSgEt_Xw4FJQxLJZgMutw3awoAq2Ve
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=12XhSgEt_Xw4FJQxLJZgMutw3awoAq2Ve' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=12XhSgEt_Xw4FJQxLJZgMutw3awoAq2Ve" -O isaid_instance_mask_train.zip && rm -rf /tmp/cookies.txt

# https://drive.google.com/file/d/1YLjZ1cmA9PH3OfzMF-eq6T-O9FTGvSrx/view?usp=sharing
#    1YLjZ1cmA9PH3OfzMF-eq6T-O9FTGvSrx
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1YLjZ1cmA9PH3OfzMF-eq6T-O9FTGvSrx' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1YLjZ1cmA9PH3OfzMF-eq6T-O9FTGvSrx" -O isaid_semantic_mask_train.zip && rm -rf /tmp/cookies.txt


echo "val data iSAID downloading...."
# https://drive.google.com/file/d/1GCExuFqEKOY5Hyp1WSAmdW6I4RAaxBGG/view?usp=sharing
#    1GCExuFqEKOY5Hyp1WSAmdW6I4RAaxBGG
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1GCExuFqEKOY5Hyp1WSAmdW6I4RAaxBGG' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1GCExuFqEKOY5Hyp1WSAmdW6I4RAaxBGG" -O isaid_instance_mask_val.zip && rm -rf /tmp/cookies.txt

# https://drive.google.com/file/d/1_PJy7kDVnp9tjETUbvwNjkLGpzDfqYh6/view?usp=sharing
#    1_PJy7kDVnp9tjETUbvwNjkLGpzDfqYh6
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1_PJy7kDVnp9tjETUbvwNjkLGpzDfqYh6' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1_PJy7kDVnp9tjETUbvwNjkLGpzDfqYh6" -O isaid_semantic_mask_val.zip && rm -rf /tmp/cookies.txt


echo "test data iSAID downloading...."
# https://drive.google.com/file/d/1fwiTNqRRen09E-O9VSpcMV2e6_d4GGVK/view?usp=sharing
#    1fwiTNqRRen09E-O9VSpcMV2e6_d4GGVK
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1fwiTNqRRen09E-O9VSpcMV2e6_d4GGVK' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1fwiTNqRRen09E-O9VSpcMV2e6_d4GGVK" -O isaid_test_1.zip && rm -rf /tmp/cookies.txt

# https://drive.google.com/file/d/1wTwmxvPVujh1I6mCMreoKURxCUI8f-qv/view?usp=sharing
#    1wTwmxvPVujh1I6mCMreoKURxCUI8f-qv
wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=1wTwmxvPVujh1I6mCMreoKURxCUI8f-qv' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=1wTwmxvPVujh1I6mCMreoKURxCUI8f-qv" -O isaid_test_2.zip && rm -rf /tmp/cookies.txt


unzip dota_v1_train.zip
mv images dota_v1_train

unzip dota_v1_val.zip
mv images dota_v1_val

unzip isaid_instance_mask_train.zip
mv images isaid_instance_mask_train

unzip isaid_semantic_mask_train.zip
mv images isaid_semantic_mask_train

unzip isaid_instance_mask_val.zip
mv images isaid_instance_mask_val

unzip isaid_semantic_mask_val.zip
mv images isaid_semantic_mask_val

unzip isaid_test_1.zip
mv images isaid_test_1

unzip isaid_test_2.zip
mv images isaid_test_2

mkdir test && cd test && mkdir images && cd ..
mkdir train && cd train && mkdir images && cd ..
mkdir val && cd val && mkdir images && cd ..

mv dota_v1_train/* train/images/.
mv isaid_instance_mask_train/* train/images/.
mv isaid_semantic_mask_train/* train/images/.

mv dota_v1_val/* val/.
mv isaid_instance_mask_val/* val/images/.
mv isaid_semantic_mask_val/* val/images/.

mv isaid_test_1/* test/images/.
mv isaid_test_2/* test/images/.


