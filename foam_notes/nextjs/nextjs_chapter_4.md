前文中我们定义了两个表，分别是user和activateToken，我们实现注册要使用两种方式，一种是短信注册，一种是账号注册，账号注册需要验证邮箱的真伪，注册账号前发送到邮箱一个code，表activate存储邮箱账号和code，提交注册时验证code，创建账号完成注册，短信注册同理，最终页面效果如图

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/86fe002d7454493f94dcea9c0eb4ff3a~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=2218&h=1512&s=575388&e=png&b=fcfcfc)

## 一、安装必要的库

首先，我们需要安装一些必要的库和组件

```bash
# 表单hook npm i react-hook-form # 表单hook形式的验证解析器 npm i @hookform/resolvers # 邮箱验证格式 npm i email-validator # 密码验证 npm i bcryptjs npm i --save-dev @types/bcryptjs
```

```bash
# 添加shadcn组件 npx shadcn-ui@latest add input npx shadcn-ui@latest add tabs npx shadcn-ui@latest add label npx shadcn-ui@latest add card npx shadcn-ui@latest add textarea npx shadcn-ui@latest add toast
```

toast（临时显示简洁消息）在使用前需要在全局layout中引入

```ts
// src/app/layout.tsx import { Toaster } from '@/components/ui/toaster' <Providers> {children} <Toaster /> </Providers>
```

手动触发方式如下：

```ts
const { toast } = useToast() toast({ title: '账号注册', description: '注册成功', variant: 'default', })
```

## 二、账号注册表单规则

定义邮箱登录注册的数据验证规则

```ts
// src/lib/validator.ts import { z } from 'zod' import emailValidator from 'email-validator' const passwordSchema = z.string().refine( (password) => password.length >= 8 && password.length <= 20 && /\d/.test(password) && // 包含数字 /[a-z]/.test(password) && // 包含小写字母 /[A-Z]/.test(password) && // 包含大写字母 /\W|_/.test(password), // 包含符号 { message: '密码必须包含数字、大写字母、小写字母和符号，长度为8 ~ 20个字符', } ) export const EmailFormVaildator = z.object({ email: z.string().refine((email) => emailValidator.validate(email), { message: '邮箱格式不正确', }), password: passwordSchema, confirmPassword: passwordSchema, emailCode: z.string().refine((code) => code.length > 0, { message: '邮箱激活码必填', }), }) export type TEmailFormVaildator = z.infer<typeof EmailFormVaildator>
```

## 三、创建注册页面

以注册为例，注册页面使用的Form是客户端组件，不把这个页面定义为客户端是因为后续需要验证，如果登录以后访问时就给重定向

```ts
// src/app/register/page.tsx import Form from './form' export default async function RegisterPage() { return <Form /> }
```

## 四、创建 Form 组件

在 `Form` 组件中，使用Tab来切换短信注册和账号注册

```ts
// src/app/register/form.tsx import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs' import EmailRegister from './EmailRegister' import PhoneRegister from './PhoneRegister' export default function Form() { return ( <> <div className="text-3xl font-bold text-center mt-10">注册</div> <Tabs defaultValue="phone" className="w-full max-w-lg mx-auto p-6"> <TabsList className="grid w-full grid-cols-2"> <TabsTrigger value="phone">短信注册</TabsTrigger> <TabsTrigger value="email">账号注册</TabsTrigger> </TabsList> <TabsContent value="phone"> <PhoneRegister /> </TabsContent> <TabsContent value="email"> <EmailRegister /> </TabsContent> </Tabs> </> ) }
```

PhoneRegister使用以下占位

```ts
// src/app/register/PhoneRegister.tsx import React from 'react' function PhoneRegister() { return <div>PhoneRegister</div> } export default PhoneRegister
```

## 五、账号注册

短信注册组件后续讲，账号注册主要实现了以下功能：

1.  **邮箱激活**：用户输入邮箱后，点击“激活”按钮，将向用户的邮箱发送激活邮件。
2.  **实时验证**：在用户输入邮箱、密码和确认密码时，组件会进行实时验证，并在输入不符合要求时显示错误信息。
3.  **邮箱注册**：用户输入正确的邮箱、密码、确认密码和邮箱激活码后，点击“注册”按钮进行注册。

组件使用了以下主要技术和库：

-   **React**：用于构建组件和处理用户交互。
-   **React Hook Form**：用于管理表单状态，包括输入验证、错误处理等。
-   **Zod**：用于进行数据验证。
-   **tRPC**：用于处理邮箱注册和邮箱激活的API请求。
-   **email-validator**：用于验证邮箱格式是否正确。

完整代码如下：

```ts
// src/app/register/EmailRegister.tsx 'use client' import { useCallback, useEffect } from 'react' import { useForm } from 'react-hook-form' import { zodResolver } from '@hookform/resolvers/zod' import emailValidator from 'email-validator' import { EmailFormVaildator, TEmailFormVaildator } from '@/lib/validator' import { Input } from '@/components/ui/input' import { Button } from '@/components/ui/button' import { trpc } from '@/app/_trpc/client' import { Loader2 } from 'lucide-react' import { Card, CardContent, CardDescription, CardFooter, CardHeader, } from '@/components/ui/card' import { Label } from '@/components/ui/label' import { Textarea } from '@/components/ui/textarea' import { cn } from '@/lib/utils' import { useToast } from '@/components/ui/use-toast' function EmailRegister() { const { toast } = useToast() const { register, handleSubmit, formState: { errors }, setValue, setError, clearErrors, getValues, watch, } = useForm<TEmailFormVaildator>({ defaultValues: {}, resolver: zodResolver(EmailFormVaildator), mode: 'all', }) const confirmPassword = watch('confirmPassword') const password = watch('password') // 使用 useEffect 在 password, confirmPassword 变化时进行验证 useEffect(() => { if (password && confirmPassword && password !== confirmPassword) { setTimeout(() => { setError('confirmPassword', { type: 'manual', message: '两次输入密码不一致', }) }) } else { clearErrors('confirmPassword') } }, [password, confirmPassword, setError, clearErrors]) const { mutate: startEmailRegister, isLoading: emailRegisterLoading } = trpc.emailRegister.useMutation({ onSuccess: (user) => { if (user && user.id) { // 注册成功 toast({ title: '账号注册', description: '注册成功', variant: 'default', }) } }, onError: (error) => { toast({ title: '账号注册', description: error.message, variant: 'destructive', }) }, }) const handleSubmitEmail = useCallback( ({ email, password, confirmPassword, emailCode }: TEmailFormVaildator) => { if (password !== confirmPassword) { setError('confirmPassword', { type: 'manual', message: '两次输入密码不一致', }) return } startEmailRegister({ email, password, emailCode }) }, [setError, startEmailRegister] ) const { mutate: startActiveEmail, isLoading: emailActiveLoading } = trpc.emailActive.useMutation({ onSuccess: (data) => { if (data && data.status === 'success') { toast({ title: '激活邮箱', description: '激活邮件已发送，请前往邮箱查看', variant: 'default', }) } }, onError: (error) => { toast({ title: '激活邮箱', description: error.message, variant: 'destructive', }) }, }) const handleActiveEmail = useCallback(() => { clearErrors('email') const email = getValues('email')?.trim() if (!email || !emailValidator.validate(email)) { setError('email', { type: 'manual', message: '请输入正确的邮箱，然后激活', }) return } startActiveEmail({ email }) }, [clearErrors, getValues, setError, startActiveEmail]) return ( <Card> <CardHeader> <CardDescription> 邮箱注册后可绑定手机号登录，先点激活更快获取激活码 </CardDescription> </CardHeader> <form id="emailRegister" onSubmit={(e) => { e.preventDefault() handleSubmit(handleSubmitEmail)() }} > <CardContent className="space-y-2"> <div className="space-y-1"> <Label className="text-zinc-600" htmlFor="email"> 邮箱： </Label> <div className="flex space-x-2"> <Input {...register('email')} onBlur={(e) => { setValue('email', e.target.value) }} className={cn(errors.email && 'focus-visible:ring-red-500')} id="email" placeholder={'输入邮箱'} autoComplete="username" type="email" /> <Button onClick={handleActiveEmail} className="min-w-max text-zinc-500" variant={'outline'} type="button" disabled={emailActiveLoading} > {emailActiveLoading && ( <Loader2 className="mr-1 h-4 w-4 animate-spin" /> )} 激活{emailActiveLoading ? '中' : ''} </Button> </div> <div className="text-destructive text-xs mt-1"> {errors.email ? errors.email.message : null} </div> </div> <div className="space-y-1"> <Label className="text-zinc-600" htmlFor="password"> 密码： </Label> <Input {...register('password')} className={cn(errors.password && 'focus-visible:ring-red-500')} onBlur={(e) => { setValue('password', e.target.value) }} id="password" placeholder={'输入登录密码'} autoComplete="new-password" type="password" /> <div className="text-destructive text-xs mt-1"> {errors.password ? errors.password.message : null} </div> </div> <div className="space-y-1"> <Label className="text-zinc-600" htmlFor="rePassword"> 确认密码： </Label> <Input {...register('confirmPassword')} className={cn( errors.confirmPassword && 'focus-visible:ring-red-500' )} onBlur={(e) => { setValue('confirmPassword', e.target.value) }} id="rePassword" placeholder={'再次输入登录密码'} autoComplete="new-password" type="password" /> <div className="text-destructive text-xs mt-1"> {errors.confirmPassword ? errors.confirmPassword.message : null} </div> </div> <div className="space-y-1"> <Label className="text-zinc-600" htmlFor="emailCode"> 激活码： </Label> <Textarea {...register('emailCode')} className={cn(errors.emailCode && 'focus-visible:ring-red-500')} placeholder={'输入邮箱激活码'} onBlur={(e) => { setValue('emailCode', e.target.value || '') }} id="emailCode" /> <div className="text-destructive text-xs mt-1"> {errors.emailCode ? errors.emailCode.message : null} </div> </div> </CardContent> <CardFooter> <Button className="w-full" type="submit" disabled={emailRegisterLoading} > {emailRegisterLoading && ( <Loader2 className="mr-4 h-4 w-4 animate-spin text-white" /> )} 注册 </Button> </CardFooter> </form> </Card> ) } export default EmailRegister
```

## 六、验证策略

[www.react-hook-form.com/api/useform…](https://link.juejin.cn/?target=https%3A%2F%2Fwww.react-hook-form.com%2Fapi%2Fuseform%2F%23mode "https://www.react-hook-form.com/api/useform/#mode")

此选项允许您在用户提交表单之前配置验证策略。验证发生在事件期间，该事件是通过调用函数触发的

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/8e19cee68ede486a97d09217d5e8edd8~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1882&h=910&s=280997&e=png&b=090f1e)

所以为了实时验证，我们使用`mode: 'all'`，验证在模糊事件和更改事件上都被触发。

## 七、完成激活注册

首先移除原来的test API，复原dashboard页面

```ts
// src/app/dashboard/page.tsx function DashboardPage() { return <div>DashboardPage</div> } export default DashboardPage
```

完成激活邮箱和注册账号的逻辑

```ts
// src/trpc/index.ts import { db } from '@/db' import { publicProcedure, router } from './trpc' import { z } from 'zod' import { TRPCError } from '@trpc/server' import { hash } from 'bcryptjs' export const appRouter = router({ emailRegister: publicProcedure .input( z.object({ email: z.string(), password: z.string(), emailCode: z.string(), }) ) .mutation(async ({ input }) => { const { email, password, emailCode } = input // 验证参数 if (!email || !password || !emailCode) { throw new TRPCError({ code: 'BAD_REQUEST', message: '客户端缺少参数', }) } // 验证邮箱是否已经注册 const user = await db.user.findFirst({ where: { email, }, }) if (user) { throw new TRPCError({ code: 'BAD_REQUEST', message: '邮箱已经注册，请直接登录', }) } // 验证激活码是否存在 const emailCodeRecord = await db.activateToken.findFirst({ where: { account: email, }, }) if (!emailCodeRecord) { throw new TRPCError({ code: 'BAD_REQUEST', message: '激活码不存在，请重新发送获取', }) } // 验证激活码是否过期 if (emailCodeRecord.expiredAt.getTime() < Date.now()) { throw new TRPCError({ code: 'BAD_REQUEST', message: '激活码已过期，请重新发送获取', }) } // 验证激活码是否正确 if (emailCodeRecord.code !== emailCode) { throw new TRPCError({ code: 'BAD_REQUEST', message: '激活码不正确，请重新输入', }) } // 创建用户 const hashedPassword = await hash(password, 10) const newUser = await db.user.create({ data: { email, password: hashedPassword, }, }) return { id: newUser.id, email } }), emailActive: publicProcedure .input(z.object({ email: z.string() })) .mutation(async ({ input }) => { const { email } = input // 创建激活码 const hashedEmail = await hash(email, 10) // 保存激活码=》不存在email就创建，存在就更新 await db.activateToken.upsert({ where: { account: email, }, create: { account: email, code: hashedEmail, expiredAt: new Date(Date.now() + 1000 * 60 * 60 * 1), // 1小时过期 }, update: { code: hashedEmail, expiredAt: new Date(Date.now() + 1000 * 60 * 60 * 1), // 1小时过期 }, }) // 发送邮件 // 返回状态 return { status: 'success' } }), }) // export type definition of API export type AppRouter = typeof appRouter
```

下一节会进行回顾和优化代码，如邮箱发送邮件，提示样式，文案，错误异常处理等。