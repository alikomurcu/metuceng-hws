SELECT *
FROM Vaccine V
WHERE V.code NOT IN(SELECT V1.code
					FROM Vaccination V1, User U
					WHERE U.userID = V1.userID)
ORDER BY V.code;