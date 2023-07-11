SELECT U1.userID, A.effectcode, A.effectname
FROM User U1, User U2, Seen S, Vaccination V1, Vaccination V2, AllergicSideEffect A
WHERE U1.userID = U2.userID AND U1.userID = V1.userID AND V1.code = V2.code AND V1.userID = V2.userID AND V1.dose = (V2.dose - 1)  AND DATEDIFF(V2.vacdate, V1.vacdate) < 20 AND S.userID = U1.userID AND S.effectcode = A.effectcode
ORDER BY A.effectcode;