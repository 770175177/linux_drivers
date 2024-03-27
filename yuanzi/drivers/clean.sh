for ((i=1; i<10; i++))
do 
	cd 0$i*
	make clean
	rm -rf .tmp*
	rm .*.swp
	rm .*.cmd
	cd ..
done

for ((i=10; i<25; i++))
do 
	cd $i*
	make clean
	rm -rf .tmp*
	rm .*.swp
	rm .*.cmd
	cd ..
done
