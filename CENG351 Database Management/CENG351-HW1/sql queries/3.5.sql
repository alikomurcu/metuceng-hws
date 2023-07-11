SELECT V1.code, V1.vaccinename, V1.type
FROM Vaccine V1
WHERE V1.vaccinename NOT LIKE '%vac%' 
	AND V1.code IN 
	(SELECT V.code
	FROM Vaccination V
	WHERE V.vacdate = (SELECT MAX(V2.vacdate) FROM Vaccination V2))
UNION
SELECT BB.code, BB.vaccinename, BB.type
FROM Vaccination Q, Vaccine BB
WHERE BB.code = Q.code AND Q.vacdate = (SELECT MAX(LAST.vacdate) FROM 
	(SELECT A.code as code, A.vacdate as vacdate
		FROM
		(SELECT V1.code as code, V1.vaccinename as vaccinename, V1.type as type
		FROM Vaccine V1
		WHERE V1.vaccinename NOT LIKE '%vac%' 
			AND V1.code IN 
			(SELECT V.code
			FROM Vaccination V
			WHERE V.vacdate = (SELECT MAX(V2.vacdate) FROM Vaccination V2))) as L, 
		(SELECT P.code as code, P.vaccinename as vaccinename, P.type as type, G.vacdate as vacdate FROM Vaccination G, Vaccine P WHERE P.code = G.code AND P.vaccinename NOT LIKE '%vac%') as A
		WHERE A.code <> L.code) # this is a huge subquery, forgive me for the complicateness, I did my best :)
		 as LAST)
ORDER BY code;