foreach ($file in Get-ChildItem -Path . -Recurse -File -Include *.vert, *.frag) {
   $command = $Env:VULKAN_SDK + '\Bin\glslc.exe'
   $output = $file.FullName + '.spv'
   Start-Process -FilePath $command -ArgumentList $file.FullName, '-o', $output
}