import sys
import random
"""
generate data from xapian index database.
score equals to id

example:
id, content, score
1, t1, 1
2, t1 t2, 2
3, t1 t2 t3, 3

if id % 1 == 0, we will add the term t1 in the content.
if id % 2 == 0, we will add the term t2 in the content.
...
if id % 20 == 0, we will add the term t20 in the content.

So, if you choose t1, you can choose all the documents.
if you choose t2, you can choose 1/2 of the documents.
if you choose t3, you can choose 1/3 of the documents.
...
if you choose t20, you can choose 1/20 of the documents.
"""
def main(doc_num):
    random.seed(1)
    # n sample
    print ",".join(["id,content,score"])
    for docid in range(1, doc_num + 1):
        content = "t1"
        for i in range(2, 21):
            if docid % i == 0:
                content += " t%s" %i

        score = docid
        data = ",".join(map(str, [docid, content, score]))
        print data
        if docid % 1000000 == 0:
            print >> sys.stderr, "current docid is ", docid
if __name__ == "__main__":
    doc_num = int(sys.argv[1])
    main(doc_num)
