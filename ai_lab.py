#MDP

class companyMDP:
    def __init__(self):
        self.N = 4
        self.states = ['PU', 'PF', 'RU', 'RF']
        self.transitions = {('PU', 'S'): {'PU': 1}, ('PU', 'A'): {'PU': 0.5, 'PF': 0.5}, 
                            ('PF', 'S'): {'PU': 0.5, 'RF': 0.5}, ('PF', 'A'): {'PF': 1},
                            ('RU', 'S'): {'RU': 0.5, 'PU': 0.5}, ('RU', 'A'): {'PU': 0.5, 'PF': 0.5},
                            ('RF', 'S'): {'RF': 0.5, 'RU': 0.5}, ('RF', 'A'): {'PF': 1}}
        self.rewards = {'PU': 0, 'PF': 0, 'RU': 10, 'RF': 10}

    def startState(self):
        return self.states[0]

    def isEnd(self, state):
        return state == self.states[self.N - 1]

    def actionsFrom(self, state):
        return ['S', 'A']

    def succProbReward(self, state, action):
        result = []
        for s_ in self.transitions[(state, action)]:
            result.append((s_, self.transitions[(state, action)][s_], self.rewards[s_]))
        return result

    def discount(self):
        return 0.9

    def getStates(self):
        return self.states

    def getReward(self, state):
        return self.rewards[state]

def valueIteration2(mdp):
    def Q(state, action):
        return sum(prob * U[newState] for newState, prob, reward in mdp.succProbReward(state, action))
                       

    # Initialize the values to zero
    U = {}
    for state in mdp.getStates():
        U[state] = mdp.getReward(state)

    i = 0
    while i < 10:
        i += 1
        newU = {}
        for state in mdp.getStates():
            newU[state] = mdp.getReward(state) + mdp.discount() * max(Q(state, action) for action in mdp.actionsFrom(state))

        # Check for convergence
        if max(abs(U[state] - newU[state]) for state in mdp.getStates()) < 1e-5:
            break

        U = newU

        # Policy
        # pi = {}
        # for state in mdp.getStates():
        #     pi[state] = max((Q(state, action), action) for action in mdp.actionsFrom(state))[1]

        # Display
        print('{:20} {:20}'.format('s', 'U(s)'))
        for state in mdp.getStates():
            print('{:20} {:20}'.format(state, U[state]))
        print("----------------------------------------")

company = companyMDP()
valueIteration2(company)

#FORWARD
H = {'A': 0.2, 'C': 0.3, 'G': 0.3, 'T': 0.2}
L = {'A': 0.3, 'C': 0.2, 'G': 0.2, 'T': 0.3}
transitionP = {('S', 'H'): 0.5, ('S', 'L'): 0.5, ('H', 'H'): 0.5, ('L', 'L'): 0.6, ('L', 'H'): 0.4, ('H', 'L'): 0.5}
seq = 'GGCA'
P = []

for i in seq:
    if len(P) == 0:
        p = [transitionP[('S', 'H')] * H[i], transitionP[('S', 'L')] * L[i]]
    else:
        p = []
        # H
        # (H -> H) + (L -> H)
        p.append(P[-1][0] * transitionP[('H', 'H')] * H[i] + P[-1][1] * transitionP[('L', 'H')] * H[i])
        # L
        # (L -> L) + (H -> L)
        p.append(P[-1][1] * transitionP[('L', 'L')] * L[i] + P[-1][0] * transitionP[('H', 'L')] * L[i])
    P.append(p)

print(P)
print()
print(P[-1][0] + P[-1][1])

#VERTIBRI
H = {'A': -2.322, 'C': -1.737, 'G': -1.737, 'T': -2.322}
L = {'A': -1.737, 'C': -2.322, 'G': -2.322, 'T': -1.737}
transitionP = {('S', 'H'): -1, ('S', 'L'): -1, ('H', 'H'): -1, ('L', 'L'): -0.737, ('L', 'H'): -1.322, ('H', 'L'): -1}
seq = 'GGCACTGAA'
P = []
parent = []

for i in seq:
    if len(P) == 0:
        p = [transitionP[('S', 'H')] + H[i], transitionP[('S', 'L')] + L[i]]
    else:
        p = []
        par = []
        # H
        p.append(H[i] + max(P[-1][0] + transitionP[('H', 'H')], P[-1][1] + transitionP[('L', 'H')]))
        if P[-1][0] + transitionP[('H', 'H')] > P[-1][1] + transitionP[('L', 'H')]:
            par.append('H')
        else:
            par.append('L')
        # L
        p.append(L[i] + max(P[-1][0] + transitionP[('H', 'L')], P[-1][1] + transitionP[('L', 'L')]))
        if P[-1][0] + transitionP[('H', 'L')] > P[-1][1] + transitionP[('L', 'L')]:
            par.append('H')
        else:
            par.append('L')

        parent.append(par)
        
    P.append(p)

# Backtracking
path = []
if P[-1][0] > P[-1][1]:
    path.extend([par[0] for par in parent])
    path.append('H')
else:
    path.extend([par[1] for par in parent])
    path.append('L')

print(path)


