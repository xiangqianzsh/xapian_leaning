def func(n):
    # n sample
    print ",".join(["order,name,score"])
    for i in range(0, n):
        docid = i + 1;
        term = "a "
        if i % 2 == 0:
            term += "b"
        data = ",".join(map(str, [docid, term, docid]))
        print data
if __name__ == "__main__":
    func(3000)


