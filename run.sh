make
make kill
# cd scripts
# source .venv/bin/activate
# cd ..

# javac -cp "./libs/mysql.jar:." src/server/Dotenv.java src/server/Encryption.java src/server/Sync.java src/server/Main.java
# cd src/server
# java -cp "../../libs/mysql.jar:." Main &
# cd ../..
# sleep 1

# rm FinanceTracker
# rm -rf build
# mkdir build
# cd build
# cmake ..
# make -j8
# mv FinanceTracker ..
# cd ..
# ./FinanceTracker

# if [[ "$OSTYPE" == "darwin"* ]]; then
#   PID=$(lsof -ti :8080)
# else
#   PID=$(ss -tulpn | awk '/:8080 / {gsub(".*pid=", "", $NF); gsub(",.*", "", $NF); print $NF}')
# fi

# if [ -n "$PID" ]; then
#   kill "$PID"
# fi
