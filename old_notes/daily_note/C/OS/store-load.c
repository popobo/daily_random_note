int x = 0, y = 0;

void T1() {
  x = 1; int t = y; // Store(x); Load(y)
  __sync_synchronize();
  printf("%d", t);
}

void T2() {
  y = 1; int t = x; // Store(y); Load(x)
  __sync_synchronize();
  printf("%d", t);
}

// 遍历模型告诉我们可能出现的结果有：01, 10, 11
// 但实际上出现00的次数是最多的，比例高达90%