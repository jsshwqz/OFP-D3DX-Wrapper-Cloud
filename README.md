# OFP D3DX Wrapper

针对 **Operation Flashpoint (闪点行动)** 的 D3D8→D3D9 转换器，提供画面增强和兼容性修复。

## 功能特性

### 核心功能
- **D3D8→D3D9转换** - 将OFP的DirectX 8指令转换为DirectX 9，提升帧数和稳定性
- **画面增强** - 各向异性过滤、抗锯齿、垂直同步控制
- **夜间滤镜修复** - 移除OFP愚蠢的夜间黑白滤镜
- **MOD支持** - 完整支持MOD_GB等mod

### 画面设置（可配置）
| 设置 | 默认值 | 说明 |
|------|--------|------|
| AnisotropicFiltering | 16 | 各向异性过滤倍数 (0-16) |
| AntiAliasing | 0 | 抗锯齿采样 (0, 2, 4, 8) |
| EnableVSync | 0 | 垂直同步 (0=禁用, 1=启用) |
| RemoveNightFilter | 1 | 移除夜间滤镜 (1=启用) |

## 安装方法

### 方法1：从源码编译

```bash
# 克隆仓库
git clone https://github.com/YOUR_USERNAME/OFP-D3DX-Wrapper-Cloud.git
cd OFP-D3DX-Wrapper-Cloud

# 创建构建目录
mkdir build
cd build

# 使用CMake配置
cmake .. -G "Visual Studio 17 2022" -A Win32

# 编译
cmake --build . --config Release
```

### 方法2：使用GitHub Actions（推荐）

1. Fork本仓库
2. 在GitHub中查看Actions标签页
3. 选择最新的build workflow
4. 下载生成的DLL文件

### 安装到游戏目录

将以下文件复制到OFP游戏目录：
```
d3d8.dll      # D3D8→D9转换器
d3dx.dll      # 画面增强
d3dx.ini      # 配置文件
```

## 使用方法

### 启动命令

```cmd
# 基础启动
FLASHPOINT1.96.EXE -nomap -nosplash -mod=@MOD_GB

# 带窗口模式
FLASHPOINT1.96.EXE -mod=@MOD_GB -window -nomap

# 带额外mod
FLASHPOINT1.96.EXE -mod=@MOD_GB;@island;@vs -nomap
```

### 配置说明

编辑 `d3dx.ini` 文件：

```ini
[d3d9]
AnisotropicFiltering=16   ; 设置各向异性过滤
AntiAliasing=0            ; 设置抗锯齿
EnableVSync=0             ; 设置垂直同步

[Compatibility]
RemoveNightFilter=1       ; 移除夜间滤镜
FixNightScene=1           ; 修复夜间场景
```

## 测试

### 测试环境
- 测试目录：`D:\ofp\OperationFlashpoint\`
- 游戏版本：OFP 1.96
- MOD：MOD_GB

### 测试步骤
1. 将编译生成的DLL文件复制到测试目录
2. 运行启动脚本
3. 检查日志文件 (d3dx.log, Flashpoint.rpt)
4. 验证游戏正常启动和运行

## 文件说明

```
OFP-D3DX-Wrapper-Cloud/
├── src/
│   ├── d3d8to9/           # D3D8→D9核心转换
│   │   ├── d3d8.cpp       # D3D8接口封装
│   │   ├── d3d9.cpp       # D3D9渲染器
│   │   └── d3d8to9.cpp    # 核心转换逻辑
│   ├── ofp_compat/        # OFP特化兼容
│   │   ├── night_filter.cpp  # 夜间滤镜移除
│   │   └── mod_support.cpp   # MOD支持
│   ├── enhancement/       # 画面增强
│   │   ├── anisotropic.cpp   # 各向异性过滤
│   │   └── vsync.cpp         # 垂直同步
│   ├── detect/           # 环境检测
│   │   ├── gpu_detect.cpp    # 显卡检测
│   │   └── fallback.cpp      # 智能降级
│   └── main.cpp          # 入口点
├── include/
│   ├── config.hpp        # 配置结构
│   └── d3d8to9.hpp       # D3D8→D9接口
├── .github/workflows/    # CI/CD配置
├── d3dx.ini             # 默认配置文件
└── CMakeLists.txt       # 构建配置
```

## 性能优化

### 推荐设置

| 场景 | 各向异性过滤 | 垂直同步 | 预期效果 |
|------|-------------|---------|---------|
| 日常游戏 | 16 | 关闭 | 最佳画质 + 高帧数 |
| 大规模战斗 | 8 | 关闭 | 平衡性能 |
| 最低配置 | 0 | 关闭 | 最大性能 |

### 已知限制

- 超过1200视距后，CPU和内存将成为瓶颈
- 海量AI单位场景性能有限
- 5000视距 + 海量单位 = 需要强大CPU/内存

## 故障排除

### 游戏无法启动
1. 检查 `Flashpoint.rpt` 日志文件
2. 确认DLL文件已正确复制
3. 尝试禁用所有增强功能 (`MinPerformanceMode=1`)

### 画面异常
1. 检查 `d3dx.log` 日志
2. 降低各向异性过滤设置
3. 禁用抗锯齿

### 性能问题
1. 关闭垂直同步
2. 降低视距设置
3. 检查显卡驱动版本

## 致谢

- **elishacloud** - D3D8to9项目原作者
- **crosire** - ReShade作者，d3d8to9维护者
- OFP社区 - 测试和反馈

## 许可证

本项目基于MIT许可证开源。

## 联系方式

- GitHub Issues: 报告bug和建议
- GitHub Discussions: 讨论和问答
