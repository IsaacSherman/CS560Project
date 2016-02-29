cd ../Commands/exp/
make -B
cd ../../JShell
clear
cp ../Commands/exp/*.h ./libfdr
cp ../Commands/exp/*.a ./libfdr
make -B
clear
