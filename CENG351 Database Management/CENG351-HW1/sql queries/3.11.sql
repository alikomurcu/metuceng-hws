UPDATE User
SET status = 'Eligible'
WHERE status = 'Not_Eligible' AND userID IN 
				(SELECT V.userID
				 FROM Vaccination V
				 WHERE (V.vacdate,V.userID) IN (SELECT MAX(V2.vacdate), V2.userID FROM Vaccination V2 GROUP BY V2.userID) AND DATEDIFF('2021-12-19', V.vacdate)>120);
				#replace the date