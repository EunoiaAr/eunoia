param (
    [string] $releaseNotes
    )

if (!(Test-path lib))      { mkdir lib }
if (!(Test-path lib\4.0 )) { mkdir lib\4.0 }

$libPath        = '..\x64\Release\Eunoia.Net.Collections.dll'
$nugetSpecPath  = 'lib\4.0\Eunoia.Net.Collections.dll'
$nuspecPath     = '.\Eunoia.Net.Collections.dll.nuspec'
$libVersion     = Get-Item $libPath | % {[Reflection.AssemblyName]::GetAssemblyName($_.FullName).Version.ToString() }
$libVersion
Copy-Item $libPath lib\4.0 
$xml            = New-Object xml
$xml.PreserveWhitespace = $true
$xml.Load($nuspecPath)

$xml.package.metadata.version = $libVersion.ToString()
if (![String]::IsNullOrWhiteSpace($releaseNotes)) { 
    $xml.package.metadata.releaseNotes = $releaseNotes 
}
Set-Content $nuspecPath -Value $xml.OuterXml

nuget pack $nuspecPath -Exclude *.ps1 -Exclude *.pfx
