#!/usr/bin/env python3
import subprocess
import time
import pandas as pd
import matplotlib.pyplot as plt
import sys

# Instâncias de teste: (N, P)
test_instances = [
    (3, 80),
    (6, 77),
    (9, 74),
    (12, 71),
    (15, 68),
    (18, 65),
    (21, 62),
    (24, 59),
    (27, 56),
    (30, 53),
]

results = []
seed = 42

print("=" * 80)
print("AVALIAÇÃO EXPERIMENTAL - PROJETO 3")
print("=" * 80)
print(f"\n{'#':<3} {'N':<5} {'P%':<5} {'m_real':<8} {'m_fut':<8} {'Vars':<8} {'Restr':<8} {'Complex':<10} {'Tempo(s)':<10}")
print("-" * 80)

for idx, (n, p) in enumerate(test_instances, 1):
    try:
        # Gerar instância
        gen_cmd = ['./gerador_p3', str(n), str(p), str(seed)]
        gen_result = subprocess.run(gen_cmd, capture_output=True, text=True, timeout=10)
        
        if gen_result.returncode != 0:
            print(f"{idx:<3} {n:<5} {p:<5} ERRO NA GERAÇÃO")
            continue
        
        input_data = gen_result.stdout
        
        # Extrair número de jogos realizados da primeira linha
        first_line = input_data.split('\n')[0].split()
        m_realizados = int(first_line[1])
        
        # Calcular jogos futuros
        total_games = n * (n - 1)
        m_futuros = total_games - m_realizados
        
        # Calcular complexidade
        num_vars = 3 * m_futuros
        num_constraints = m_futuros + (n - 1)
        complexity = num_vars + num_constraints
        
        # Executar solver e medir tempo
        start_time = time.time()
        solve_cmd = ['python3', 'project3.py']
        solve_result = subprocess.run(solve_cmd, input=input_data, capture_output=True, text=True, timeout=30)
        elapsed_time = time.time() - start_time
        
        if solve_result.returncode != 0:
            print(f"{idx:<3} {n:<5} {p:<5} {m_realizados:<8} {m_futuros:<8} ERRO NA EXECUÇÃO")
            continue
        
        # Guardar resultados
        results.append({
            'idx': idx,
            'N': n,
            'P': p,
            'm_realizados': m_realizados,
            'm_futuros': m_futuros,
            'variaveis': num_vars,
            'restricoes': num_constraints,
            'complexidade': complexity,
            'tempo': elapsed_time
        })
        
        print(f"{idx:<3} {n:<5} {p:<5} {m_realizados:<8} {m_futuros:<8} {num_vars:<8} {num_constraints:<8} {complexity:<10} {elapsed_time:<10.4f}")
        
    except subprocess.TimeoutExpired:
        print(f"{idx:<3} {n:<5} {p:<5} TIMEOUT")
    except Exception as e:
        print(f"{idx:<3} {n:<5} {p:<5} ERRO: {e}")

print("-" * 80)
print(f"\nTotal de instâncias testadas: {len(results)}\n")

if not results:
    print("ERRO: Nenhuma instância foi executada com sucesso!")
    sys.exit(1)

# Criar DataFrame
df = pd.DataFrame(results)

# Salvar tabela em CSV
csv_file = 'resultados_performance.csv'
df.to_csv(csv_file, index=False)
print(f"✓ Tabela salva em: {csv_file}")

# Criar tabela formatada para LaTeX/Markdown
print("\n" + "=" * 80)
print("TABELA FORMATADA PARA RELATÓRIO")
print("=" * 80)
print(f"\n{'#':<3} | {'N':<5} | {'P%':<5} | {'m_fut':<8} | {'Variáveis':<10} | {'Restrições':<12} | {'Complexidade':<13} | {'Tempo(s)':<10}")
print("-" * 80)
for _, row in df.iterrows():
    print(f"{row['idx']:<3} | {row['N']:<5} | {row['P']:<5} | {row['m_futuros']:<8} | {row['variaveis']:<10} | {row['restricoes']:<12} | {row['complexidade']:<13} | {row['tempo']:<10.4f}")

# Criar gráfico
plt.figure(figsize=(12, 7))
plt.plot(df['complexidade'], df['tempo'], 'bo-', linewidth=2, markersize=8, label='Tempo medido')
plt.xlabel('Complexidade (Variáveis + Restrições)', fontsize=13, fontweight='bold')
plt.ylabel('Tempo de Execução (segundos)', fontsize=13, fontweight='bold')
plt.title('Avaliação Experimental: Tempo vs Complexidade do Programa Linear', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3, linestyle='--')
plt.legend(fontsize=11)

# Adicionar anotações para alguns pontos
step = max(1, len(df) // 5)
for i in range(0, len(df), step):
    row = df.iloc[i]
    plt.annotate(f'N={row["N"]}', 
                xy=(row['complexidade'], row['tempo']),
                xytext=(10, 10), textcoords='offset points',
                fontsize=9, alpha=0.7,
                bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.3))

plt.tight_layout()
graph_file = 'grafico_performance.png'
plt.savefig(graph_file, dpi=300, bbox_inches='tight')
print(f"\n✓ Gráfico salvo em: {graph_file}")

# Imprimir estatísticas
print("\n" + "=" * 80)
print("RESUMO ESTATÍSTICO")
print("=" * 80)
print(f"Tempo mínimo:        {df['tempo'].min():.4f}s")
print(f"Tempo máximo:        {df['tempo'].max():.4f}s")
print(f"Tempo médio:         {df['tempo'].mean():.4f}s")
print(f"Tempo mediano:       {df['tempo'].median():.4f}s")
print(f"Complexidade mín:    {df['complexidade'].min()}")
print(f"Complexidade máx:    {df['complexidade'].max()}")
print("=" * 80)

print("\n✓ Avaliação experimental concluída com sucesso!")
print(f"✓ Arquivos gerados: {csv_file}, {graph_file}")
