[[ "${BASH_SOURCE[0]}" == "${0}" ]] && echo "script ${BASH_SOURCE[0]} must be sourced ..." && exit

#usage:
# fn_stoponerror "$?" $LINENO
# or (opens xed at error line)
# fn_stoponerror "$?" $LINENO xed

fn_stoponerror () {
    #lNo=$(expr $2 - 1)
    er=$1
    lNo=$2
    show=$3
    clear=$4
    #fn_interrupt $lNo
    if [ $er -ne 0 ]; then
        printf "\n$me: line $lNo: error [$er]\n"
        if [ $show = "show" ]; then
            printf "\n$me: opening xed at error line"
            xed $dirSrc/$me +$lNo
        fi
        exit $er
    else        
        if [ $clear = "clear" ]; then
            clear
        fi
        printf "\n$me: line $lNo: success.\n\n "
    fi
}

fn_dir_del_if () {
if [ -d "$1" ]; then
    printf "\n$me: removing dir $1"
    rm -r "$1"
else 
    printf "\n$me: dir $1 not exist"
fi
}

fn_interrupt () {
    condition=1756
    lNo=$1
    if [ "$dirDistr" == "" ]; then
        return
    fi
    if [ "$me" != "make_all_dir.sh" ]; then
        echo "$me: is not make_all_dir.sh"
        return
    fi
    filesNow=$(find $dirDistr -type f,d | wc -l)
    if [ "$filesNow" -gt "$condition" ]; then
        echo "$me: $lNo: files quantity [$filesNow] are greater than $condition"
        printf "\n$me: opening xed at error line\n"
        xed $dirSrc/$me +$lNo
        exit 1
    fi
}
