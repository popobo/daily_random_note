## 一、数据库服务

在本地启动数据库服务，本例中我们使用 Docker 容器来启动 MySQL 数据库服务，并使用 docker-compose 进行编排。因此，你需要预先安装 Docker 和 docker-compose。

### 1\. 安装 Docker 和 docker-compose

在 macOS 上，你可以通过 Homebrew 来安装 Docker 和 docker-compose：

```bash
brew install --cask docker brew install docker-compose
```

然后，你可以通过访问 Docker Desktop 应用程序来启动 Docker。

在 Windows 上，你需要下载并安装 Docker Desktop。请注意，Docker Desktop 需要 Windows 10 专业版或更高版本。如果你的 Windows 版本不满足这个要求，你可能需要直接安装 MySQL 服务。

你可以从 Docker 官网下载 Docker Desktop 的安装包，并按照安装向导进行安装。

### 2\. 创建 docker-compose 配置文件

创建一个名为 `docker-compose.db.yml` 的 docker-compose 配置文件：

```yaml
version: '3' services: db: image: mysql:8.0 container_name: db-mysql restart: always volumes: - ./db-data:/var/lib/mysql command: --default-authentication-plugin=mysql_native_password ports: - '3306:3306' env_file: .env environment: TZ: Asia/Shanghai
```

这个配置文件的主要内容包括：

-   使用 MySQL 8.0 作为数据库镜像
-   容器名称为 `db-mysql`
-   设置容器启动失败时自动重启
-   将主机的 `./db-data` 目录挂载到容器的 `/var/lib/mysql` 目录，用于存储数据库文件
-   设置 MySQL 使用 `mysql_native_password` 作为默认的身份验证插件
-   将容器的 3306 端口映射到主机的 3306 端口
-   使用 `.env` 文件作为环境变量文件
-   设置容器的时区为 `Asia/Shanghai`

### 3\. 创建环境变量文件

创建一个 `.env` 文件，用于存储数据库连接的敏感信息：

```ini
# ====.env==== # Mysql：docker-compose.db.yml中的环境变量 MYSQL_ROOT_PASSWORD="123456" MYSQL_USER="dbuser" MYSQL_PASSWORD="dbpwd" MYSQL_DATABASE="nextdb" # Connect database DATABASE_URL="mysql://dbuser:dbpwd@localhost:3306/nextdb" # ====.env.local/.env.production==== # Server PRO_URL="https://www.example.com" PORT="3000"
```

当前的环境配置包含了 MySQL 的 root 密码、用户名、密码和数据库名称，以及数据库连接 URL、服务器的 URL 和端口号，注意数据库连接url与其他变量相互对应。

```ini
DATABASE_URL="mysql://[name]:[pwd]@localhost:[port]/[db]"
```

特别注意：添加`.env`、`.env.production`和`/db-data`到`.gitignore`文件

### 4\. 创建 Makefile 文件

创建一个 Makefile 文件，用于方便地启动和停止 Docker 容器：

```makefile
db-start: docker-compose -f docker-compose.db.yml --compatibility up -d db-down: docker-compose -f docker-compose.db.yml down
```

你可以通过 `make db-start` 命令来启动 MySQL 容器服务，通过 `make db-down` 命令来停止 MySQL 容器服务。

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/acd52f1c0d71456380eb8728a5d20e40~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1352&h=312&s=76337&e=png&b=131313)

### 5\. 验证Mysql

使用`docker ps`，查看容器的启动状态

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/f4304296217e424aa78d576274142799~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1816&h=86&s=34057&e=png&b=131313)

接下来可以使用客户端进行连接，如mac使用DBeaver 社区版来查看数据库

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/aac77324875744db8bee0ad8c67570eb~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1668&h=504&s=357787&e=png&b=fcfcfc)

也可以不安装，后面我们使用Prisma提供的工具来查看

## 二、Prisma

Prisma 是一个开源的数据库工具，它可以帮助开发者更方便地操作数据库。以下是如何安装和配置 Prisma 的步骤：

### 1\. 安装 Prisma

首先，我们需要安装 Prisma 和 Prisma 客户端。可以通过以下命令进行安装：

```css
npm i prisma @prisma/client
```

参考：[www.prisma.io/docs/gettin…](https://link.juejin.cn/?target=https%3A%2F%2Fwww.prisma.io%2Fdocs%2Fgetting-started%2Fsetup-prisma%2Fstart-from-scratch%2Frelational-databases-typescript-mysql "https://www.prisma.io/docs/getting-started/setup-prisma/start-from-scratch/relational-databases-typescript-mysql")

### 2\. 初始化 Prisma

初始化 Prisma，如果你要使用的是 MySQL 数据库，可以使用以下命令：

```bash
npx prisma init --datasource-provider mysql
```

初始化后会创建一个`prisma/schema.prisma`文件，它定义了你的数据库模型和Prisma客户端的生成配置。

安装VsCode插件配置高亮

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/bf1c9d8093c94390896a40eaa5cb60f8~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1308&h=278&s=69897&e=png&b=181818)

### 3\. 配置数据库连接

默认情况下，初始化使会自动在 `.env` 文件添加`DATABASE_URL`，因为在前文已经配置过，所以初始化会忽略，也不会添加注释信息，如果不是使用docker安装的数据库，就按照以下格式配置MySQL 连接URL，其他的参数也要留下，因为后续部署到生产环境时将用到。

```ini
DATABASE_URL="mysql://[name]:[pwd]@localhost:[port]/[db]"
```

### 4\. 同步数据模型

在 `prisma/schema.prisma` 文件中定义数据模型，例如：

```dart
model User { id String @id @default(cuid()) name String? phone String? @unique email String? @unique password String? image String? bio String? expiredAt DateTime? active Boolean @default(true) createdAt DateTime @default(now()) } model ActivateToken { id String @id @default(cuid()) account String @unique code String expiredAt DateTime }
```

使用以下命令将改动推送到数据库：

```bash
npx prisma db push
```

`npx prisma db push` 命令将在 Prisma schema 文件中定义的数据模型推送到数据库。这个命令会自动创建或更新数据库表结构以匹配你的数据模型。

如果你想要保存这次的数据库迁移记录，可以使用以下命令：

```bash
npx prisma migrate dev --name init
```

更新模型后需要生成 Prisma 客户端，你可以使用 `npx prisma generate` 命令进行更新。

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/4ef2ba59d3fb485cb1f7b4dbc290d21e~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1414&h=776&s=162105&e=png&b=141414)

### 5\. 数据库查看

你可以使用 Prisma Studio 或者 DBeaver 社区版来查看数据库。使用以下命令可以打开 Prisma Studio：

```bash
npx prisma studio
```

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/1b67abe0066f4e3792632710477b87a2~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1344&h=846&s=55719&e=png&b=ffffff)

为了方便使用，我们把上述提到的命令都加入到Makefile文件中

```bash
pr-st: npx prisma studio pr-ge: npx prisma generate pr-pu: npx prisma db push pr-mi: npx prisma migrate dev --name init pr-de: npx prisma migrate deploy
```

### 6\. 操作数据库

创建一个可重复使用的 Prisma 客户端：

```ts
// src/db/index.ts import { PrismaClient } from '@prisma/client' declare global { var cachedPrisma: PrismaClient } let prisma: PrismaClient if (process.env.NODE_ENV === 'production') { prisma = new PrismaClient() } else { if (!global.cachedPrisma) { global.cachedPrisma = new PrismaClient() } prisma = global.cachedPrisma } export const db = prisma
```

在 `src/trpc/trpc.ts` 文件中直接使用 `db` 对象进行数据库操作。例如，更新test API查询所有user表中的数据：

```ts
import { db } from '@/db' import { publicProcedure, router } from './trpc' export const appRouter = router({ test: publicProcedure.query(async () => { await db.user.create({ data: { email: String(Math.random()), password: String(Math.random()), }, }) const res = await db.user.findMany() return res }), // ... }) // export type definition of API export type AppRouter = typeof appRouter
```

```ts
// 更新src/app/dashboard/page.tsx <p>{data}</p> // 更新为 <div>{data && data.map((item) => <p key={item.id}>{item.email}</p>)}</div>
```

### 7\. 总结Prisma开发过程

当模型发生变化时，需要生成客户端。在 Prisma 中，每次你的数据模型发生变化时，你都需要重新生成 Prisma 客户端。这是因为 Prisma 客户端是基于你的数据模型生成的，当模型发生变化时，客户端的代码也需要更新以反映这些变化，使用`npx prisma generate`可以更新。

-   方法1，快速开发阶段，不会生成迁移记录：

```bash
npx prisma db push
```

-   这个命令自动执行 `npx prisma generate`。
-   方法2，提交代码部署前，生成迁移记录：

```bash
npx prisma migrate dev --name init
```

-   这个命令自动执行 `npx prisma generate`。
-   在生产环境中部署，你可能需要使用以下命令：

```bash
npx prisma generate npx prisma migrate deploy
```

-   `npx prisma migrate deploy` 命令会应用你的 Prisma 迁移到生产数据库，但不会自动生成 Prisma 客户端，所以需要先执行 `npx prisma generate`。

总结：快速开发使用方法1，提交代码部署前使用方法2，生产环境部署使用 `npx prisma generate` 和 `npx prisma migrate deploy`。