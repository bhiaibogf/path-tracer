group = []
mtl_count = []
mtl_names = []


def add_group(filename):
    lines = open(filename, 'r').readlines()
    for line in lines:
        if line.startswith('usemtl'):
            mtl_names.append(line.split()[1])

    for line in lines:
        if line.startswith('usemtl'):
            mtl_name = line.split()[1]
            mtl_count.append(mtl_name)
            if mtl_names.count(mtl_name) > 1:
                group.append(mtl_name + str(mtl_count.count(mtl_name)))
            else:
                group.append(mtl_name)
    print(group)

    idx = 0
    with open(filename, 'w') as f:
        for line in lines:
            if line.startswith('g'):
                f.write('g ' + group[idx] + '\n')
                idx += 1
            else:
                f.write(line)


def main():
    add_group('bedroom/bedroom.obj')


if __name__ == '__main__':
    main()
