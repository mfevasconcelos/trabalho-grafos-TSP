#!/bin/bash

clear

CYAN='\033[0;36m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' 

EXE_PATH="$PWD/tsp" 
PASTA_INSTANCIAS="$PWD/instancias"
PASTA_SAIDAS="$PWD/saidas"

if [ ! -f "$EXE_PATH" ]; then
    if [ -f "$PWD/tsp.exe" ]; then
        EXE_PATH="$PWD/tsp.exe"
    else
        echo -e "${RED}ERRO: O ficheiro executavel não foi encontrado em: $EXE_PATH${NC}"
        exit 1
    fi
fi

if [ ! -d "$PASTA_INSTANCIAS" ]; then
    echo -e "${RED}ERRO: A pasta 'instancias' não foi encontrada!${NC}"
    exit 1
fi

if [ ! -d "$PASTA_SAIDAS" ]; then
    mkdir -p "$PASTA_SAIDAS"
    echo -e "${GREEN}Pasta 'saidas' criada com sucesso.${NC}\n"
fi

shopt -s nullglob
instancias=("$PASTA_INSTANCIAS"/*.tsp)

if [ ${#instancias[@]} -eq 0 ]; then
    echo -e "${YELLOW}Nenhum ficheiro .tsp encontrado na pasta de instancias.${NC}"
    exit 1
fi

for caminho_entrada in "${instancias[@]}"; do
    
    nome_arquivo=$(basename "$caminho_entrada")
    caminho_saida="$PASTA_SAIDAS/$nome_arquivo.tour"
    
    echo -n -e "A executar: $nome_arquivo ... "
    
    { time "$EXE_PATH" < "$caminho_entrada" > "$caminho_saida"; } 2>&1 | grep real | awk -v verde="${GREEN}" -v amarelo="${YELLOW}" -v reset="${NC}" '{print verde "Concluido em " reset amarelo $2 reset}'
done