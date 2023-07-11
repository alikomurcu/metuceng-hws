SELECT DISTINCT U.userID, U.userName, U.address
FROM User U, Vaccination V1, Vaccination V2
WHERE V1.code <> V2.code AND V1.userID = U.userID AND V2.userID = U.userID
 AND V1.vacdate <= '2021-12-19' AND V1.vacdate >= '2021-01-01'
  AND V2.vacdate <= '2021-12-19' AND V2.vacdate >= '2021-01-01'

 ORDER BY U.userID;