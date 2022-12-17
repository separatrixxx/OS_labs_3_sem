# Вариант 13:

**Child1 переводит строки в нижний регистр. Child2 превращает все пробельные
символы в символ «_»**

![image](https://user-images.githubusercontent.com/87654656/190494441-1ba5df58-2e2e-4b8b-953d-4376ed64a475.png)

**Для запуска:**

```
gcc -std=c99 -pedantic -Wall child1.c -o child1
gcc -std=c99 -pedantic -Wall child2.c -o child2
gcc -std=c99 -pedantic -Wall main.c
./a.out
```

**strace:**

`strace ./a.out`
