SELECT DISTINCT U.userID, U.username, U.address
FROM User U, Vaccination V
WHERE V.userID = U.userID AND V.dose >= 2 AND EXISTS 
(SELECT S.userID
FROM Seen S
WHERE S.userID = U.userID AND S.code = V.code
GROUP BY S.userID
HAVING COUNT(*)<2 )


UNION

SELECT DISTINCT Us.userID, Us.username, Us.address
FROM User Us, Vaccination Vc
WHERE Vc.userID = Us.userID AND Vc.dose >= 2 AND Us.userID NOT IN 
	(SELECT S.userID
	FROM Seen S) # NOT IN yap


ORDER BY userID;
