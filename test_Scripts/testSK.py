from sklearn import svm
X = [[0,0],[0,1],[1,0],[1,1]]
y = [0,1,1,0]
clf = svm.SVC()
print(clf.fit(X,y))
print(clf.predict([[1,0]]))