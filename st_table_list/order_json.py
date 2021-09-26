import json

def order_json(path):
    with open(path, 'r') as f:
        matrix = json.load(f)
    new_matrix = []
    for group_id, group in enumerate(matrix):
        new_matrix.append({})
        for key_id, key in enumerate(group.keys()):
            new_matrix[group_id][str(key_id)+key] = group[key]
    with open(path, 'w') as f:
        json.dump(new_matrix, f) 

if __name__ == '__main__':
    order_json('src_all_st_matrix.json')
    order_json('dst_all_st_matrix.json')