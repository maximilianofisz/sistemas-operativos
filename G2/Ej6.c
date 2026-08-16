atomic<int> count(0);

preparado();

count.getAndInc();
while (count < n) {
    continue
}

critica()