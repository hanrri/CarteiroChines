from pathlib import Path
import json

def getData(file: str, dir="dados") -> dict:
    
        arquivo_json = Path(__file__).parent.parent / dir / file
        print(arquivo_json)
        
        with open(arquivo_json, "r", encoding="utf-8") as f:
            dados = json.load(f)
            
        return dados