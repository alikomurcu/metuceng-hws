SELECT U.userID, U.userName, U.address
FROM User U
WHERE NOT EXISTS
(
SELECT V.code
FROM Vaccination V
WHERE NOT EXISTS
(SELECT DISTINCT V.code
FROM Seen S, Vaccination V, AllergicSideEffect A
WHERE S.effectcode = A.effectcode AND A.effectname = 'loss_of_speech' AND V.code = S.code AND V.userID = U.userID # change the loss of speech to the variable
))
ORDER BY U.userID;