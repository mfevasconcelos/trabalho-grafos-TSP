Clear-Host

$exePath = Join-Path $PWD "tsp.exe"
$pasta_instancias = Join-Path $PWD "instancias"
$pasta_saidas = Join-Path $PWD "saidas"

if (-Not (Test-Path $exePath)) {
    Write-Host "ERRO: O ficheiro executavel não foi encontrado em: $exePath" -ForegroundColor Red
    exit
}

if (-Not (Test-Path $pasta_instancias)) {
    Write-Host "ERRO: A pasta 'instancias' não foi encontrada!" -ForegroundColor Red
    exit
}

if (-Not (Test-Path $pasta_saidas)) {
    New-Item -ItemType Directory -Force -Path $pasta_saidas | Out-Null
    Write-Host "Pasta 'saidas' criada com sucesso.`n" -ForegroundColor Green
}

$instancias = Get-ChildItem -Path $pasta_instancias -Filter *.tsp

if ($instancias.Count -eq 0) {
    Write-Host "Nenhum ficheiro .tsp encontrado na pasta de instancias." -ForegroundColor Yellow
    exit
}

foreach ($arquivo in $instancias) {
    Write-Host "A executar: $($arquivo.Name) ... " -NoNewline
    
    $caminho_entrada = $arquivo.FullName
    $caminho_saida = Join-Path $pasta_saidas "$($arquivo.Name).tour"
    
    $tempo = Measure-Command {
        Start-Process -FilePath $exePath -RedirectStandardInput $caminho_entrada -RedirectStandardOutput $caminho_saida -Wait -NoNewWindow
    }
    
    Write-Host "Concluido em " -NoNewline -ForegroundColor Green
    Write-Host "$([math]::Round($tempo.TotalSeconds, 4)) segundos." -ForegroundColor Yellow
}