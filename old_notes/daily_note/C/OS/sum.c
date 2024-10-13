#define N 100000000
long sum = 0;

void Tsum() { for (int i = 0; i < N; i++) sum++; }

int main() {
  create(Tsum);
  create(Tsum);
  join();
  printf("sum = %ld\n", sum);
}

// 如果添加编译优化？
// -O1: 100000000 😱😱
// -O2: 200000000 😱😱😱

/*

while (!done);
// would be optimized to
if (!done) while (1);

*/