
rm log_coin.txt logSealed_DES.txt

for (( threads = 1; threads <= 16; threads = $((threads+1)) ))
do
    echo "Num threads = $threads"

    for(( flips = 100000000; flips <= 1600000000; flips =$((flips+100000000)) ))
    do
        echo "Now doing $flips flips."
        java ParallelCoinFlip $threads $flips >> log_coin.txt
    done
done


echo 
echo 
echo "Staring SealedDES benchmark..."
for (( threads = 1; threads <= 16; threads = $((threads+1)) ))
do
    echo "Num threads = $threads"

    for(( bitSize = 15; bitSize <= 25; bitSize =$((bitSize+1)) ))
    do
        echo "Now decrypting $bitSize bits."
        java SealedDESParallel $threads $bitSize >> log_Sealed_DES.txt
    done
done
