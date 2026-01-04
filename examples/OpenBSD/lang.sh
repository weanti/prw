#!/bin/ksh

# 2 modes: query mode and toggle mode
# in query mode the actual language is printed
# in toggle mode a list of languages can be defined. If the current language is in the list then the next to it will be the new language (list overflow works)
#   if the current language is not in the list then the first language will be the new language
# no arg -> query mode
# args : toggle between given languages
LANG_LIST=$@
LANG_CNT=$#
KBLANG=`setxkbmap -query | grep "layout" | awk '{print $2}'`
if [ $LANG_CNT -ne 0 ]
then
    if [ "$1" = "-h" ]
    then
        echo $0 "[lang0 [lang1 [lang2 ... ] ] | -h]"
        echo "Prints current keyboard layout if no argument is given."
        echo "lang0 [lang1 [lang2 ...] ]: Toggles between given languages. e.g. if current language is us and $0 us de fr is invoked then it switches to de, if invoked again it switched to fr and if invoked again it switched to us again. If Current language is not in the specified list, then it switches to the first on in the list. The script does not check if the given languages are valid or not."
        echo "-h: Prints this help."
        return
    fi
    # find current language in the list and change to the next one or the first one if not found
    PREV=""
    for LANG in $LANG_LIST
    do
        if [ "$PREV" = "$KBLANG" ]
        then
            setxkbmap $LANG
            echo  $LANG
            return
        fi
        PREV=$LANG
    done
    # not found or it is the last one, then switch to the first
    setxkbmap $1
    echo $1
    return
fi
# query mode, with \c no newline is printed
echo $KBLANG "\c"
