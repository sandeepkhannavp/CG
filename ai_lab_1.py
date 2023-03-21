def forward1(trans, emission, pi, seq):
    m = len(seq)
    n = pi.shape[0]
    
    alpha = np.zeros((m, n))
    alpha[0, :] = pi * emission[:, seq[0]]
    
    for t in range(1, m):
        for i in range(n):
            for j in range(n):
                alpha[t, j] = alpha[t-1][i] * trans[i][j] * emission[j][seq[t]]
    
    print(np.sum(alpha[m-1:]))

def viterbi1(trans, emission, pi, seq):
    m = len(seq)
    n = pi.shape[0]
    
    omega = np.zeros((m, n))
    omega[0, :] = np.log(pi * emission[:, seq[0]])
    prev = np.zeros((m-1, n))
    
    for i in range(1, m):
        for j in range(n):
            prob = omega[i-1] + np.log(trans[:, j]) + np.log(emission[j, seq[i]])
            prev[i-1, j] = np.argmax(prob)
            omega[i, j] = np.max(prob)
    s = np.zeros(m)
    last = np.argmax(omega[m-1, :])
    s[0] = last
    
    btIndex = 1
    for i in range(m-2, -1, -1):
        s[btIndex] = prev[i, int(last)]
        last = prev[i, int(last)]
        btIndex += 1
        
    s = np.flip(s, axis=0)
    result = []
    for i in s:
        if i==0:
            result.append('H')
        else:
            result.append('L')
    print(result)

#Transition probabilities
transProb = np.array([[0.5, 0.5], [0.4, 0.6]])

#Emission probabilities
emission = np.array([[0.2, 0.3, 0.3, 0.2], [0.3, 0.2, 0.2, 0.3]])

#Initial probabilities
initial = np.array([0.5, 0.5])

#A->0
#C->1
#G->2
#T->3

forward1(transProb, emission, initial, [2, 2, 1, 0])
viterbi1(transProb, emission, initial, [2, 2, 1, 0, 1, 3, 2, 0, 0])

rewards = {'Sun': 4, 'Wind': 0, 'Hail': -8}
transitions = {'Sun': {1: [('Sun', 0.5), ('Wind', 0.5)]},
               'Wind': {1: [('Sun', 0.5), ('Hail', 0.5)]},
               'Hail': {1: [('Wind', 0.5), ('Hail', 0.5)]}}

# rewards = {'PU': 0, 'PF': 0, 'RU': 10, 'RF': 10}
# transitions = {'PU': {'S': [('PU', 1)],
#                       'A': [('PU', 0.5), ('PF', 0.5)]},
#                'PF': {'S': [('PU', 0.5), ('RF', 0.5)],
#                       'A': [('PF', 1)]},
#                'RU': {'S': [('PU', 0.5), ('RU', 0.5)],
#                       'A': [('PU', 0.5), ('PF', 0.5)]},
#                'RF': {'S': [('RU', 0.5), ('RF', 0.5)],
#                       'A': [('PF', 1)]}}

d1, d2 = dict(rewards), dict(rewards)
discountFactor = 0.9
tolerance = 1e-3

count = 0
while True:
    for i in transitions:
        m = -float('inf')
        for j in transitions[i]:
            m = max(m, sum([k[1] * d2[k[0]] for k in transitions[i][j]]))
        d1[i] = rewards[i] + discountFactor * m
    delta = max([abs(d1[k] - d2[k]) for k in d1])
    if delta < tolerance: break
    d2 = dict(d1)
    count += 1
print(d1, count)

