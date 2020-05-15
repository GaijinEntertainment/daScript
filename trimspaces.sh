src_folders=( include src utils examples/contDev examples/daScript examples/pathTracer examples/test )
for i in "${src_folders[@]}"
do
	echo "> processing $i"
	./trim_folder_spaces.sh $i
done
