for loop in 1 2
do
    echo "Testcase $loop"
    cp text$loop.in text.in
    cp vocab$loop.in vocab.in
    ./tokenizer-en.exe
    echo "run tokenizer-en.exe"
    if [ diff result.out result$loop.out ] then
        echo "Find difference!!"
    else
        echo "Accepted"
    fi
done