import csv

query_file = open("query", "w+")

query_file.write("delete customer;\n")
query_file.write("create customer (string name, string country, uint_32 order_id);\n")

customer_file = open("customer.csv")
string_mask = [True, True, False]
customer_reader = csv.reader(customer_file, delimiter=',')
for row in customer_reader:
    roww = "in customer insert ("
    for i in range(0, len(string_mask)):
        if (string_mask[i]):
            roww += '\"'
        roww += row[i]
        if (string_mask[i]):
            roww += '\"'
        roww += ", "

    roww = roww[:-2] + ");\n"
    query_file.write(roww)



query_file.write("delete order;\n")
query_file.write("create order (uint_32 id, float freight, uint_32 shipvia, string country);\n")

order_file = open("order.csv")
string_mask = [False, False, False, True]
order_reader = csv.reader(order_file, delimiter=',')
for row in order_reader:
    roww = "in order insert ("
    for i in range(0, len(string_mask)):
        if (string_mask[i]):
            roww += '\"'
        roww += row[i]
        if (string_mask[i]):
            roww += '\"'
        roww += ", "

    roww = roww[:-2] + ");\n"
    query_file.write(roww)

query_file.write("exit\n")


# from c in customer join o in order on (c.order_id == o.id) & (o.freight < 1.0) & (o.shipvia == 3) select c.name, o.country;