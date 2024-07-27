# Setup
mkdir exports
cd exports
pip3 install h5py

# Download SIFT (1000000 x 128)
wget ftp://ftp.irisa.fr/local/texmex/corpus/sift.tar.gz
tar -xzvf sift.tar.gz
rm sift.tar.gz

# Download GIST (1000000 x 960)
wget ftp://ftp.irisa.fr/local/texmex/corpus/gist.tar.gz
tar -xzvf gist.tar.gz
rm gist.tar.gz

# Download GloVe (1000000 x 200)
mkdir glove
cd glove 
wget https://nlp.stanford.edu/data/glove.twitter.27B.zip
unzip glove.twitter.27B.zip glove.twitter.27B.200d.txt
python3 ../../utils/txt_to_fvecs.py glove.twitter.27B.200d.txt
python3 ../../utils/split_fvecs.py glove.fvecs glove_base.fvecs temp.fvecs 1000000 110000
python3 ../../utils/split_fvecs.py temp.fvecs glove_learn.fvecs glove_query.fvecs 100000 10000
rm glove.twitter.27B.zip glove.twitter.27B.200d.txt glove.fvecs temp.fvecs
cd ..

# Download Deep (1000000 x 96)
mkdir deep
cd deep
git clone https://github.com/matsui528/deep1b_gt.git
cd deep1b_gt
python3 download_deep1b.py --root ./deep1b --base_n 1 --learn_n 1 --ops query base learn gt
python3 pickup_vecs.py --src ./deep1b/base/base_00 --dst ./deep1b/deep1M_base.fvecs --topk 1000000
python3 pickup_vecs.py --src ./deep1b/learn/learn_00 --dst ./deep1b/deep1M_learn.fvecs --topk 100000
cd ..
mv deep1b_gt/deep1b/deep1M_base.fvecs deep_base.fvecs
mv deep1b_gt/deep1b/deep1M_learn.fvecs deep_learn.fvecs
mv deep1b_gt/deep1b/deep1B_queries.fvecs deep_query.fvecs
mv deep1b_gt/deep1b/deep1B_groundtruth.ivecs deep_groundtruth.ivecs
rm -rf ./deep1b_gt
cd ../..