SELECT DISTINCT U.userID, U.username, U.address
FROM User U, Vaccination V
WHERE U.userID = V.userID AND V.userID IN (SELECT V2.userID
										   FROM Vaccination V2
										   WHERE V2.dose = 2 AND V2.userID IN (SELECT V3.userID
																				FROM Vaccination V3
																				WHERE V3.dose = 1 AND V3.vacdate > '2021-10-01'))
ORDER BY U.userID;