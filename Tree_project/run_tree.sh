#!/bin/bash
clear
echo "Removing any existing files"
make -f 'make_tree_exe' clean
make -f 'make_texitree' clean
rm bin/tree.txt
rm bin/proto_tree.txt
echo "Making new executables"
make -f 'make_tree_exe' || exit
make -f 'make_texitree' || exit
echo -e "\n"

title="Tree types"
prompt="Pick an option by typing its corresponding number followed by return:"
options=("AVL" "WAVL" "2-3" "2-3-4")

echo -e "$title\n"
PS3="$prompt "
select opt in "${options[@]}" "Quit"; do

    case "$REPLY" in

        1 ) echo "You picked $opt which is option $REPLY";
            bin/tree_exe avl data/data.txt;
            bin/texitree text < bin/proto_tree.txt > bin/tree.txt;
            "${EDITOR:-nano}" bin/tree.txt;
            echo -e "\n";;
        2 ) echo "You picked $opt which is option $REPLY";
            bin/tree_exe wavl data/data.txt;
            bin/texitree text < bin/proto_tree.txt > bin/tree.txt;
            "${EDITOR:-nano}" bin/tree.txt;
            echo -e "\n";;
        3 ) echo "You picked $opt which is option $REPLY";
            bin/tree_exe 2-3 data/data.txt;
            bin/texitree text < bin/proto_tree.txt > bin/tree.txt;
            "${EDITOR:-nano}" bin/tree.txt;
            echo -e "\n";;
        4 ) echo "You picked $opt which is option $REPLY";
            bin/tree_exe 2-3-4 data/data.txt;
            bin/texitree text < bin/proto_tree.txt > bin/tree.txt;
            "${EDITOR:-nano}" bin/tree.txt;
            echo -e "\n";;

        $(( ${#options[@]}+1 )) ) echo "Goodbye!"; exit;;
        *) echo "Invalid option. Try another one."; continue;;

    esac

done
