for ((i=1; i<10; i++))
do 
	cd 0$i*
	make clean
	rm  *code-workspace
	rm  *.imx
	rm  -rf .vscode
	cd ..
done

for ((i=10; i<22; i++))
do 
	cd $i*
	make clean
	rm  *code-workspace
	rm  *.imx
	rm  -rf .vscode
	cd ..
done
