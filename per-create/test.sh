for i in {1..2}
do
    echo "Test $i th time"
    ./client vcm-14287.vm.duke.edu 12345 > client$i.txt
done
echo "Done!"
