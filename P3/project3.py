import sys
from pulp import *

def get_solver():
    try:
        s = PULP_CBC_CMD(msg=False, timeLimit=10)
        if s.available():
            return s
    except:
        pass
    try:
        s = GLPK_CMD(msg=False)
        if s.available():
            return s
    except:
        pass
    return None

def main():
    data = sys.stdin.read().split()
    if not data:
        return

    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    #pontos atuais
    pontos = [0] * (n + 1)
    jogos_realizados = set()

    for _ in range(m):
        a = int(next(it))
        b = int(next(it))
        r = int(next(it))

        jogos_realizados.add((a, b))

        if r == 0:
            pontos[a] += 1
            pontos[b] += 1
        elif r == a:
            pontos[a] += 3
        elif r == b:
            pontos[b] += 3

    #jogos futuros 
    jogos_futuros = [
        (i, j)
        for i in range(1, n + 1)
        for j in range(1, n + 1)
        if i != j and (i, j) not in jogos_realizados
    ]

    #jogos futuros por equipa
    jogos_futuros_por_equipa = [[] for _ in range(n + 1)]
    total_jogos_futuros = [0] * (n + 1)

    for (a, b) in jogos_futuros:
        jogos_futuros_por_equipa[a].append((a, b))
        jogos_futuros_por_equipa[b].append((a, b))
        total_jogos_futuros[a] += 1
        total_jogos_futuros[b] += 1

    solver = get_solver()
    respostas = []

    for equipa in range(1, n + 1):

        if total_jogos_futuros[equipa] == 0:
            if all(pontos[equipa] >= pontos[k] for k in range(1, n + 1) if k != equipa):
                respostas.append(0)
            else:
                respostas.append(-1)
            continue

        max_pontos = pontos[equipa] + 3 * total_jogos_futuros[equipa]
        if any(max_pontos < pontos[k] for k in range(1, n + 1) if k != equipa):
            respostas.append(-1)
            continue

        if all(pontos[equipa] >= pontos[k] for k in range(1, n + 1) if k != equipa):
            respostas.append(0)
            continue

        #criar LP
        prob = LpProblem(f"E{equipa}", LpMinimize)

        W, D, L = {}, {}, {}

        for (a, b) in jogos_futuros:
            W[(a, b)] = LpVariable(f"W{a}_{b}_{equipa}", cat="Binary")
            D[(a, b)] = LpVariable(f"D{a}_{b}_{equipa}", cat="Binary")
            L[(a, b)] = LpVariable(f"L{a}_{b}_{equipa}", cat="Binary")
            prob += W[(a, b)] + D[(a, b)] + L[(a, b)] == 1

        pontos_expr = {}
        for k in range(1, n + 1):
            termos = [pontos[k]]
            for a, b in jogos_futuros:
                if a == k:
                    termos.extend([3 * W[(a, b)], D[(a, b)]])
                elif b == k:
                    termos.extend([3 * L[(a, b)], D[(a, b)]])
            pontos_expr[k] = lpSum(termos)

        for k in range(1, n + 1):
            if k != equipa:
                prob += pontos_expr[equipa] >= pontos_expr[k]

        #minimizar vitórias da equipa
        prob += lpSum([W[(a, b)] if a == equipa else L[(a, b)]
                       for (a, b) in jogos_futuros_por_equipa[equipa]])

        status = prob.solve(solver)

        if LpStatus[status] == "Optimal":
            respostas.append(int(value(prob.objective)))
        else:
            respostas.append(-1)

    print("\n".join(map(str, respostas)))

if __name__ == "__main__":
    main()