# shared_bike
共享单车


![image](https://github.com/1AoB/shared_bike/assets/78208268/8a89d729-286c-480b-82a0-7bca29e4e4a5)

# 演示效果
## 管理员登录
![QQ录屏20240412173012 -original-original](https://github.com/1AoB/shared_bike/assets/78208268/77db1473-91ef-4c24-a1fa-0ded1070d9dd)
## 用户注册与登录
![QQ录屏20240412173500 -original-original](https://github.com/1AoB/shared_bike/assets/78208268/5096c72c-74d9-4d8c-96f4-3fe6510ca75a)



## 用户骑行


# 压力测试
![image](https://github.com/1AoB/shared_bike/assets/78208268/b72b02c5-c5cf-4573-acb8-7db37832c400)


- "Webbench - Simple Web Benchmark 1.5"：Webbench 工具的名称和版本号。
- "Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software."：Webbench 工具的版权声明。
- "Benchmarking: GET http://192.168.52.128:9090/ ↗"：对哪个 URL 进行性能测试。
- "2000 clients, running 5 sec."：使用 2000 个客户端并发请求，在 5 秒内进行测试。
- "Speed=831144 pages/min, 0 bytes/sec."：测试结果，其中 Speed 表示测试的速度，以每分钟处理的页面数为单位。0 bytes/sec 表示测试期间没有传输任何数据量。
- "Requests: 69262 susceed, 0 failed."：测试期间成功和失败的请求数量。
根据测试结果，Webbench 工具在 5 秒内使用 2000 个并发客户端，以每分钟处理 831144 个页面的速度测试了给定 URL。此外，在测试期间共发送了 69262 个请求，并且没有失败的请求，这表明被测试的服务在给定的测试条件下能够处理大量请求而不出现问题。需要注意的是，Webbench 工具是一个相对简单的性能测试工具，实际的性能表现可能会受到多种因素的影响。
