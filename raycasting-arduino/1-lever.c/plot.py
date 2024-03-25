import matplotlib.pyplot as plt

thetas = []
rs = []
with open('data.txt', 'r') as fp:
    for line in fp:
        theta, r = line.split(' ')
        # thetas.append(int(theta) / 360 * 2 * 3.14)
        thetas.append(int(theta))
        rs.append(int(r))

print(thetas)
print(rs)

fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
ax.plot(thetas,rs)
plt.show()