SELECT AVG(N.dose)
FROM (SELECT COUNT(V.dose) as dose
FROM Vaccination V, User U
WHERE V.userID = U.userID AND U.age > 65
GROUP BY U.userID) AS N