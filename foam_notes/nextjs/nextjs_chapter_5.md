## 一、提示样式

优化前：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ce12e204369b4e74914ba898af476d4d~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1606&h=304&s=427167&e=png&b=f6f6f6)

我们可能需要根据不同的情境展示不同的图标。这可以通过判断`variant`参数来实现，如下所示

```ts
// src/components/ui/toaster.tsx {props.variant === 'default' ? ( <CheckCircle className="text-primary" /> ) : ( <CircleOff /> )}
```

优化后：

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/bebdfcbc85774bb6816d2398a25054db~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1684&h=308&s=484546&e=png&b=f7f7f7)

对于安装的shadcn组件，都是可以自行修改的，package中虽然有记录，但是重新安装包时也不会覆盖，当你想再次添加该组件时会询问`Component toast already exists. Would you like to overwrite?`

## 二、提示文案

以下是整理的注册相关的提示内容，导出了一个`getMessages`方法，会根据`code`参数返回对应的提示文案，并在需要的时候替换文案中的字段。`validatorMessages`内是格式校验文案直接引入即可。

```ts
// src/lib/tips.ts const messages = { '9997': '密码必须包含数字、大写字母、小写字母和符号，长度为8 ~ 20个字符', '9998': '邮箱激活码必填', '9999': '服务端走神了，请联系管理员', '10000': '未知错误', '10001': '邮箱格式不正确', '10002': '两次输入密码不一致', '10003': '账号注册', '10004': '用户${field}注册成功', '10005': '激活邮箱', '10006': '激活邮件已发送，请前往邮箱查看', '10007': '请输入正确的邮箱，然后激活', '10008': '输入邮箱', '10009': '输入登录密码', '10010': '再次输入登录密码', '10011': '输入邮箱激活码', '10012': '客户端缺少参数', '10013': '邮箱已经注册，请直接登录', '10014': '激活码不存在，点击激活获取', '10015': '激活码已过期，请重新发送获取', '10016': '激活码不正确，请重新输入', '10017': '发送邮件失败，请检查邮箱是否正确', } export const validatorMessages = { email: messages['10001'], password: messages['9997'], emailCode: messages['9998'], } export type TipsCode = keyof typeof messages export const getMessages = (code: TipsCode, field?: string) => { if (field && messages[code]) return messages[code].replace('${field}', field) return messages[code] ? messages[code] : messages['10000'] }
```

## 三、异常处理

在处理异常时，如果是主动抛出的错误，我们可以直接抛出错误信息；如果不是主动抛出的错误，为了防止暴露敏感信息，我们可以抛出一个固定的服务端错误提示。以下是一个示例

```ts
// src/lib/utils.ts import { TRPCError } from '@trpc/server' import { getMessages } from './tips' import { TRPC_ERROR_CODE_KEY } from '@trpc/server/rpc' export class ManualTRPCError extends TRPCError { constructor(code: TRPC_ERROR_CODE_KEY, message?: string) { super({ code: code, message: message, }) this.name = 'ManualTRPCError' } } export function handleErrorforInitiative(error: any) { if (error instanceof ManualTRPCError) { throw error } else { throw new ManualTRPCError('INTERNAL_SERVER_ERROR', getMessages('9999')) } }
```

这段代码首先定义了一个`ManualTRPCError`类，该类继承自`TRPCError`类，然后定义了一个`handleErrorforInitiative`函数，该函数接受一个错误对象作为参数，如果错误对象是`ManualTRPCError`的实例，则直接抛出该错误；否则，抛出一个新的`ManualTRPCError`错误，并设置错误信息为服务端错误提示。

在trpc的路由中使用trycatch捕获错误示例：

```ts
emailRegister: publicProcedure .input( z.object({ email: z.string(), password: z.string(), emailCode: z.string(), }) ) .mutation(async ({ input }) => { try { const { email, password, emailCode } = input // 验证参数 if (!email || !password || !emailCode) { throw new ManualTRPCError('BAD_REQUEST', getMessages('10012')) } // ... return { id: newUser.id, email } } catch (error) { handleErrorforInitiative(error) } }),
```

同时完成emailRegister和emailActive的错误处理

## 四、发送邮件

我们可以使用`nodemailer`库来发送邮件，安装所需的包

```bash
npm i nodemailer npm i --save-dev @types/nodemailer
```

以QQ邮箱为例，根据以下获取授权码，然后进行环境变量配置

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/cd98c1c138874bc089c18f59e0b0300a~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1872&h=794&s=256422&e=png&b=e9f2f8)

`.env.local`中配置以下环境变量，`USER`和`PASS`是变化的，其他两个不需要变，`PASS`是申请的授权码，同时配置到`.env.example`

```ini
# email-send EMAIL_HOST="smtp.qq.com" EMAIL_PORT="587" EMAIL_USER="...@qq.com" EMAIL_PASS="..." DOMAIN="example.com"
```

创建一个发送邮件的工具函数，后续默认完成了message的文案

```ts
// src/lib/sendEmail.ts import nodemailer from 'nodemailer' import { ManualTRPCError } from './utils' import { getMessages } from './tips' const transporter = nodemailer.createTransport({ host: process.env.EMAIL_HOST, port: Number(process.env.EMAIL_PORT), secure: false, auth: { user: process.env.EMAIL_USER, //我的邮箱 pass: process.env.EMAIL_PASS, //授权码 }, }) interface IEmailOptions { to: string subject: string text?: string html?: string } export const sendEmail = async ({ to, subject, text, html }: IEmailOptions) => { try { const info = await transporter.sendMail({ from: process.env.EMAIL_USER, // sender address to, // list of receivers subject, // Subject line text, // plain text body html, // html body }) return info } catch (error) { // 失败时候的处理 throw new ManualTRPCError('BAD_REQUEST', getMessages('10017')) } }
```

创建邮件模板

```ts
// src/lib/utils.ts export function getEmailTemplate(hashedEmail: string, sendEmail: string) { const sendInfo = { hashedemail: hashedEmail, url: process.env.NEXTAUTH_URL, domain: process.env.DOMAIN, sendEmail: sendEmail, a: '您好', b: '欢迎您注册为我们的用户，以下是验证秘钥：', c: '为了您的安全，秘钥将在24小时后过期。', d: '如果不是您本人注册为我们的用户，请安全的忽略该邮件。', e: '这个信息是从', f: '发出到', } return ` <div class="mailMainArea" style="font-size: 14px; font-family: Verdana, 宋体, Helvetica, sans-serif; line-height: 1.66; padding: 8px 10px; margin: 0px; width: 700px;"><table border="0" cellpadding="0" cellspacing="0" class="" style="border-collapse: separate; mso-table-lspace: 0pt; mso-table-rspace: 0pt; width: 100%;"> <tbody><tr> <td style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top;">&nbsp;</td> <td class="" style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top; display: block; Margin: 0 auto; max-width: 540px; padding: 10px; width: 540px;"> <div class="" style="box-sizing: border-box; display: block; Margin: 0 auto; max-width: 600px; padding: 30px 20px;"> <span class="" style="color: transparent; display: none; height: 0; max-height: 0; max-width: 0; opacity: 0; overflow: hidden; mso-hide: all; visibility: hidden; width: 0;">Welcome back to Build SaaS with Ethan!</span> <table class="" style="border-collapse: separate; mso-table-lspace: 0pt; mso-table-rspace: 0pt; width: 100%; background: #ffffff; border-radius: 8px;"> <tbody><tr> <td class="" style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top; box-sizing: border-box;"> <table border="0" cellpadding="0" cellspacing="0" style="border-collapse: separate; mso-table-lspace: 0pt; mso-table-rspace: 0pt; width: 100%;"> <tbody><tr> <td style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top;"> <p style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 20px; color: #15212A; font-weight: bold; line-height: 24px; margin: 0; margin-bottom: 15px;">${sendInfo.a}</p> <p style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 16px; color: #3A464C; font-weight: normal; margin: 0; line-height: 24px; margin-bottom: 32px;">${sendInfo.b}</p> <table border="0" cellpadding="0" cellspacing="0" class="" style="border-collapse: separate; mso-table-lspace: 0pt; mso-table-rspace: 0pt; width: 100%; box-sizing: border-box;"> <tbody> <tr> <td align="left" style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 16px; vertical-align: top; padding-bottom: 35px;"> <table border="0" cellpadding="0" cellspacing="0" style="border-collapse: separate; mso-table-lspace: 0pt; mso-table-rspace: 0pt; width: auto;"> <tbody> <tr> <td style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 16px; vertical-align: top; background-color: #3B82F6; border-radius: 5px; text-align: center;"> <p style="display: inline-block; color: #ffffff; background-color: #3B82F6; border: solid 1px #3B82F6; border-radius: 5px; box-sizing: border-box; cursor: pointer; text-decoration: none; font-size: 16px; font-weight: normal; margin: 0; padding: 9px 22px 10px; border-color: #3B82F6;" _act="check_domail">${sendInfo.hashedemail}</p> </td> </tr> </tbody> </table> </td> </tr> </tbody> </table> <p style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 16px; color: #3A464C; font-weight: normal; line-height: 24px; margin: 0; margin-bottom: 11px;">${sendInfo.c}</p> <hr> </td> </tr> <tr> <td style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top; padding-top: 80px;"> <p class="" style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; line-height: 16px; font-size: 11px; color: #738A94; font-weight: normal; margin: 0;">${sendInfo.d}</p> </td> </tr> <tr> <td style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top; padding-top: 2px;"> <p class="" style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; line-height: 16px; font-size: 11px; color: #738A94; font-weight: normal; margin: 0;">${sendInfo.e}<a target="_blank" class="" href="${sendInfo.url}" style="text-decoration: underline; color: #738A94; font-size: 11px;" _act="check_domail">${sendInfo.domain}</a> ${sendInfo.f} <span style="text-decoration: underline; color: #738A94; font-size: 11px;" _act="check_domail">${sendInfo.sendEmail}</span></p> </td> </tr> </tbody></table> </td> </tr> </tbody></table> </div> </td> <td style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; font-size: 14px; vertical-align: top;">&nbsp;</td> </tr> </tbody></table></div> ` }
```

存储邮件主题文案

```ts
// src/lib/tips.ts const messages = { '9995': '恭喜你发现了宝藏', '9996': 'ITShareNotes官网注册', // ... } export const emailMessages = { SUBJECT: messages['9996'], TEXT: messages['9995'], }
```

我们创建了邮件模板和存储邮件主题文案，在激活邮件方法中调用`sendEmail`

```ts
// 发送邮件 await sendEmail({ to: email, subject: emailMessages.SUBJECT, text: emailMessages.TEXT, html: getEmailTemplate(hashedEmail, email), })
```

## 五、英文邮箱

默认qq邮箱的前缀都是数字，他支持我们免费注册英文邮箱，注册后EMAIL\_USER就可以使用英文邮箱进行配置，收到的邮件也是由英文邮箱发送过来的

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/e3f64ee1d54a4c56bd7dcecba8e01fe9~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1216&h=428&s=65830&e=png&b=e2edf7)

![](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/1dd9c99ba05c4e7b8ec79ae7abbf3275~tplv-k3u1fbpfcp-jj-mark:3024:0:0:0:q75.awebp#?w=1414&h=812&s=159210&e=png&b=faf9f9)