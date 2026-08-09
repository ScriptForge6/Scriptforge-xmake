<#
.SYNOPSIS
统计当前目录下的代码文件行数（忽略空行和单行注释）
#>

# 自动使用当前运行脚本的目录作为项目根目录（无需手动修改）
$projectPath = $PWD.Path

# 要统计的文件类型（可根据需要增减）
$fileExtensions = @(".ixx", ".json", ".cpp", ".h", ".hpp")

# 初始化统计结果
$totalLines = 0
$fileStats = @{}

# 遍历指定类型的文件（处理中文路径和权限问题）
Get-ChildItem -Path $projectPath -Recurse -File -ErrorAction SilentlyContinue | Where-Object {
    $fileExtensions -contains $_.Extension
} | ForEach-Object {
    $filePath = $_.FullName
    $extension = $_.Extension
    
    try {
        # 读取文件内容，兼容不同编码
        $lines = Get-Content -Path $filePath -Encoding UTF8 -ErrorAction Stop
        $codeLines = $lines | Where-Object {
            # 去除首尾空格后非空，且不是单行注释
            $trimmed = $_.Trim()
            $trimmed -ne "" -and -not $trimmed.StartsWith("//")
        }
        
        $lineCount = $codeLines.Count
        $totalLines += $lineCount
        
        # 按文件类型汇总
        if ($fileStats.ContainsKey($extension)) {
            $fileStats[$extension] += $lineCount
        } else {
            $fileStats[$extension] = $lineCount
        }
        
        # 输出单个文件的统计结果（可选，方便核对）
        Write-Host "$filePath : $lineCount 行"
    }
    catch {
        Write-Warning "无法读取文件 $filePath : $($_.Exception.Message)"
    }
}

# 输出汇总结果
Write-Host "`n===== 代码行数统计汇总 ====="
foreach ($ext in $fileStats.Keys) {
    Write-Host "$ext 文件: $($fileStats[$ext]) 行"
}
Write-Host "`n总计有效代码行数: $totalLines 行"