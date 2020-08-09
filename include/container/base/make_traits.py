int_type_list = ['char', 'short', 'int', 'long', 'long long']
float_type_list = ['float', 'double', 'long double']
unsigned_type_list = ['unsigned ' + x for x in int_type_list]

traits_list = ['pod', 'move']
traits_inner_list = ['is_pod', 'has_move_ctor']
traits_type_list = ['true', 'false']

for index in range(len(traits_list)):
    for type_list in [int_type_list, unsigned_type_list, float_type_list]:
        for typeStr in type_list:
            traits_str = "template <> struct " + traits_list[index] + "_traits<" + typeStr + "> { using " + traits_inner_list[index] + " = " + traits_type_list[index] + "_type; };"
            print(traits_str)
    print("")
