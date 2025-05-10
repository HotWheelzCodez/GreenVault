javac -cp ./libs/mysql.jar src/server/Dotenv.java src/server/Encryption.java src/server/Sync.java src/server/Main.java
cd src/server
java -cp ../../libs/mysql.jar Main
cd ../..
rmdir /s /q build
mkdir build
cd build
cmake .. -A x64
cmake --build . --parallel
cd Debug
move FinanceTracker.exe ../..
cd ../..
FinanceTracker
