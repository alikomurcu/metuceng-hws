package ceng.ceng351.cengvacdb;

//import com.mysql.cj.protocol.Resultset;

//import javax.xml.transform.Result;
import java.sql.*;

public class CENGVACDB implements ICENGVACDB
{
    private String uname = "****";
    private String pword = "****";
    private String host = "****";
    private String db = "****";
    private int port = 8080;
    private Connection con = null;

    public CENGVACDB()
    {

    }
    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.db + "?useSSL=false";
        /*
        try{
            Class.forName("com.mysql.jdbc.Driver");
        }
        catch(ClassNotFoundException e)
        {
            System.out.println("No Mysql connector!");
            e.printStackTrace();
        }*/
        try
        {
            Class.forName("com.mysql.cj.jdbc.Driver");
            this.con = DriverManager.getConnection(url, uname, pword);
        }
        catch (SQLException | ClassNotFoundException i)
        {
            System.out.println("Connection unsuccesful.");
            i.printStackTrace();
        }
        if(con != null)
        {
            System.out.println("Succesful");

        }
        else
        {
            System.out.println("Unsuccesful");
        }
    }

    @Override
    public int createTables() {
        Statement statement = null;
        int c = 0;
        try {
            statement = con.createStatement();
            statement.executeUpdate("CREATE TABLE `db`.`User` (" +
                    "  `userID` INT NOT NULL," +
                    "  `userName` VARCHAR(30) NULL," +
                    "  `age` INT NULL," +
                    "  `address` VARCHAR(150) NULL," +
                    "  `password` VARCHAR(30) NULL," +
                    "  `status` VARCHAR(45) NULL," +
                    "  PRIMARY KEY (`userID`));");
            c += 1;
            statement.executeUpdate("CREATE TABLE `db`.`Vaccine` (" +
                    "  `code` INT NOT NULL," +
                    "  `vaccineName` VARCHAR(30) NULL," +
                    "  `type` VARCHAR(30) NULL," +
                    "  PRIMARY KEY (`code`));");
            c += 1;
            statement.executeUpdate("CREATE TABLE `db`.`Vaccination` (" +
                    "  `code` INT NOT NULL," +
                    "  `userID` INT NOT NULL," +
                    "  `dose` INT NOT NULL," +
                    "  `vacdate` DATE NULL," +
                    "  PRIMARY KEY (`code`, `userID`, `dose`)," +
                    "    FOREIGN KEY (code)" +
                    "    REFERENCES Vaccine (code)" +
                    "    ON DELETE CASCADE" +
                    "    ON UPDATE NO ACTION," +
                    "    FOREIGN KEY (userID)" +
                    "    REFERENCES User (userID)" +
                    "    ON DELETE NO ACTION" +
                    "    ON UPDATE NO ACTION);");
            c += 1;
            statement.executeUpdate("CREATE TABLE `db`.`AllergicSideEffect` (" +
                    "  `effectcode` INT NOT NULL," +
                    "  `effectname` VARCHAR(50) NULL," +
                    "  PRIMARY KEY (`effectcode`));");
            c += 1;
            statement.executeUpdate("CREATE TABLE `db`.`Seen` (" +
                    "  `effectcode` INT NOT NULL," +
                    "  `code` INT NOT NULL," +
                    "  `userID` INT NOT NULL," +
                    "  `date` DATE NULL," +
                    "  `degree` VARCHAR(30) NULL," +
                    "  PRIMARY KEY (`effectcode`, `code`, `userID`)," +
                    "  " +
                    "    FOREIGN KEY (`effectcode`)" +
                    "    REFERENCES `AllergicSideEffect` (`effectcode`)" +
                    "    ON DELETE CASCADE" +
                    "    ON UPDATE NO ACTION," +
                    "  " +
                    "    FOREIGN KEY (`code`)" +
                    "    REFERENCES `Vaccination` (`code`)" +
                    "    ON DELETE CASCADE" +
                    "    ON UPDATE NO ACTION," +
                    "  " +
                    "    FOREIGN KEY (`userID`)" +
                    "    REFERENCES `User` (`userID`)" +
                    "    ON DELETE NO ACTION" +
                    "    ON UPDATE NO ACTION);");
            c += 1;
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return c;
    }

    @Override
    public int dropTables() {
        Statement statement = null;
        int c = 0;
        try
        {
            statement = con.createStatement();
            statement.executeUpdate("DROP TABLE `Seen`;");
            c += 1;
            statement.executeUpdate("DROP TABLE `Vaccination`;");
            c += 1;
            statement.executeUpdate("DROP TABLE `AllergicSideEffect`;");
            c += 1;
            statement.executeUpdate("DROP TABLE `Vaccine`;");
            c += 1;
            statement.executeUpdate("DROP TABLE `User`;");
            c += 1;
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return c;
    }

    @Override
    public int insertUser(User[] users) {
        Statement statement = null;
        int c = 0;
        try
        {
            statement = con.createStatement();
            for (int i=0; i<users.length; i++)
            {
                statement.executeUpdate("INSERT INTO User " +
                        "VALUES (" +
                        users[i].getUserID() + ", " +
                        "'" + users[i].getUserName() + "', "+
                        users[i].getAge() + ", " +
                        "'" + users[i].getAddress() + "', " +
                        "'" + users[i].getPassword()+ "', " +
                        "'" + users[i].getStatus() + "'" +
                        ");");
                c += 1;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Inserting error!");
        }

        return c;
    }

    @Override
    public int insertAllergicSideEffect(AllergicSideEffect[] sideEffects) {
        Statement statement = null;
        int c = 0;
        try
        {
            statement = con.createStatement();
            for (int i=0; i<sideEffects.length; i++)
            {
                statement.executeUpdate("INSERT INTO AllergicSideEffect " +
                        "VALUES (" +
                        sideEffects[i].getEffectCode() + ", " +
                        "'" + sideEffects[i].getEffectName() + "'"+
                        ");");
                c += 1;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Inserting error!");
        }

        return c;    }

    @Override
    public int insertVaccine(Vaccine[] vaccines) {
        Statement statement = null;
        int c = 0;
        try
        {
            statement = con.createStatement();
            for (int i=0; i<vaccines.length; i++)
            {
                statement.executeUpdate("INSERT INTO Vaccine " +
                        "VALUES (" +
                        vaccines[i].getCode() + ", " +
                        "'" + vaccines[i].getVaccineName() + "', "+
                        "'" + vaccines[i].getType() + "'" +
                        ");");
                c += 1;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Inserting error!");
        }

        return c;    }

    @Override
    public int insertVaccination(Vaccination[] vaccinations) {
        Statement statement = null;
        int c = 0;
        try
        {
            statement = con.createStatement();
            for (int i=0; i<vaccinations.length; i++)
            {
                statement.executeUpdate("INSERT INTO Vaccination " +
                        "VALUES (" +
                        vaccinations[i].getCode() + ", " +
                        vaccinations[i].getUserID() + ", " +
                        vaccinations[i].getDose() + ", " +
                        "'" + vaccinations[i].getVacdate() + "'" +
                        ");");
                c += 1;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Inserting error!");
        }

        return c;
    }

    @Override
    public int insertSeen(Seen[] seens) {
        Statement statement = null;
        int c = 0;
        try
        {
            statement = con.createStatement();
            for (int i=0; i<seens.length; i++)
            {
                statement.executeUpdate("INSERT INTO Seen " +
                        "VALUES (" +
                        seens[i].getEffectcode() + ", " +
                        seens[i].getCode() + ", " +
                        seens[i].getUserID() + ", " +
                        "'" + seens[i].getDate() + "', " +
                        "'" + seens[i].getDegree() + "'" +
                        ");");
                c += 1;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Inserting error!");
        }

        return c;
    }

    @Override
    public Vaccine[] getVaccinesNotAppliedAnyUser() {
        Statement statement = null;
        int s = 0;
        try
        {
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT *\n" +
                    "FROM Vaccine V\n" +
                    "WHERE V.code NOT IN(SELECT V1.code\n" +
                    "\t\t\t\t\tFROM Vaccination V1, User U\n" +
                    "\t\t\t\t\tWHERE U.userID = V1.userID)\n" +
                    "ORDER BY V.code;;");
            while (resultSet.next()) s += 1;  // Go to the end
            Vaccine[] vac = new Vaccine[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                Vaccine tmp = new Vaccine(resultSet.getInt("code"), resultSet.getString("vaccinename"), resultSet.getString("type"));
                vac[i] = tmp;
                i += 1;
            }
            return vac;
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("3.3 error!");
        }
        return new Vaccine[0];
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersforTwoDosesByDate(String vacdate) {
        Statement statement = null;
        int s = 0;
        try{
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT DISTINCT U.userID, U.username, U.address\n" +
                    "FROM User U, Vaccination V\n" +
                    "WHERE U.userID = V.userID AND V.userID IN (SELECT V2.userID\n" +
                    "\t\t\t\t\t\t\t\t\t\t   FROM Vaccination V2\n" +
                    "\t\t\t\t\t\t\t\t\t\t   WHERE V2.dose = 2 AND V2.userID IN (SELECT V3.userID\n" +
                    "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tFROM Vaccination V3\n" +
                    "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tWHERE V3.dose = 1 AND  V3.vacdate   > "+ "'" + vacdate + "'" +"))\n" +
                    "ORDER BY U.userID;");
            while (resultSet.next()) s += 1;  // Go to the end
            QueryResult.UserIDuserNameAddressResult[] res = new QueryResult.UserIDuserNameAddressResult[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                QueryResult.UserIDuserNameAddressResult tmp = new QueryResult.UserIDuserNameAddressResult(resultSet.getString("userID"), resultSet.getString("userName"), resultSet.getString("address"));
                res[i] = tmp;
                i += 1;
            }
            return res;
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.4 error!");
        }
        return new QueryResult.UserIDuserNameAddressResult[0];
    }

    @Override
    public Vaccine[] getTwoRecentVaccinesDoNotContainVac() {
        Statement statement = null;
        int s = 0;
        try
        {
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT V1.code, V1.vaccinename, V1.type\n" +
                    "FROM Vaccine V1\n" +
                    "WHERE V1.vaccinename NOT LIKE '%vac%' \n" +
                    "\tAND V1.code IN \n" +
                    "\t(SELECT V.code\n" +
                    "\tFROM Vaccination V\n" +
                    "\tWHERE V.vacdate = (SELECT MAX(V2.vacdate) FROM Vaccination V2))\n" +
                    "UNION\n" +
                    "SELECT BB.code, BB.vaccinename, BB.type\n" +
                    "FROM Vaccination Q, Vaccine BB\n" +
                    "WHERE BB.code = Q.code AND Q.vacdate = (SELECT MAX(LAST.vacdate) FROM \n" +
                    "\t(SELECT A.code as code, A.vacdate as vacdate\n" +
                    "\t\tFROM\n" +
                    "\t\t(SELECT V1.code as code, V1.vaccinename as vaccinename, V1.type as type\n" +
                    "\t\tFROM Vaccine V1\n" +
                    "\t\tWHERE V1.vaccinename NOT LIKE '%vac%' \n" +
                    "\t\t\tAND V1.code IN \n" +
                    "\t\t\t(SELECT V.code\n" +
                    "\t\t\tFROM Vaccination V\n" +
                    "\t\t\tWHERE V.vacdate = (SELECT MAX(V2.vacdate) FROM Vaccination V2))) as L, \n" +
                    "\t\t(SELECT P.code as code, P.vaccinename as vaccinename, P.type as type, G.vacdate as vacdate FROM Vaccination G, Vaccine P WHERE P.code = G.code AND P.vaccinename NOT LIKE '%vac%') as A\n" +
                    "\t\tWHERE A.code <> L.code) # this is a huge subquery, forgive me for the complicateness, I did my best :)\n" +
                    "\t\t as LAST)\n" +
                    "ORDER BY code;");
            while (resultSet.next()) s += 1;  // Go to the end
            Vaccine[] vac = new Vaccine[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                Vaccine tmp = new Vaccine(resultSet.getInt("code"), resultSet.getString("vaccinename"), resultSet.getString("type"));
                vac[i] = tmp;
                i += 1;
            }
            return vac;
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("3.5 error!");
        }
        return new Vaccine[0];
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersAtHasLeastTwoDoseAtMostOneSideEffect() {
        Statement statement = null;
        int s = 0;
        try{
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT DISTINCT U.userID, U.username, U.address\n" +
                    "FROM User U, Vaccination V\n" +
                    "WHERE V.userID = U.userID AND V.dose >= 2 AND EXISTS \n" +
                    "(SELECT S.userID\n" +
                    "FROM Seen S\n" +
                    "WHERE S.userID = U.userID AND S.code = V.code\n" +
                    "GROUP BY S.userID\n" +
                    "HAVING COUNT(*)<2 )\n" +
                    "\n" +
                    "\n" +
                    "UNION\n" +
                    "\n" +
                    "SELECT DISTINCT Us.userID, Us.username, Us.address\n" +
                    "FROM User Us, Vaccination Vc\n" +
                    "WHERE Vc.userID = Us.userID AND Vc.dose >= 2 AND Us.userID NOT IN \n" +
                    "\t(SELECT S.userID\n" +
                    "\tFROM Seen S) # NOT IN yap\n" +
                    "\n" +
                    "\n" +
                    "ORDER BY userID;\n");
            while (resultSet.next()) s += 1;  // Go to the end
            QueryResult.UserIDuserNameAddressResult[] res = new QueryResult.UserIDuserNameAddressResult[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                QueryResult.UserIDuserNameAddressResult tmp = new QueryResult.UserIDuserNameAddressResult(resultSet.getString("userID"), resultSet.getString("userName"), resultSet.getString("address"));
                res[i] = tmp;
                i += 1;
            }
            return res;
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.6 error!");
        }
        return new QueryResult.UserIDuserNameAddressResult[0];
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersWithAllVaccinesCanCauseGivenSideEffect(String effectname) {
        Statement statement = null;
        int s = 0;
        try{
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT U.userID, U.userName, U.address\n" +
                    "FROM User U\n" +
                    "WHERE NOT EXISTS\n" +
                    "(\n" +
                    "SELECT V.code\n" +
                    "FROM Vaccination V\n" +
                    "WHERE NOT EXISTS\n" +
                    "(SELECT DISTINCT V.code\n" +
                    "FROM Seen S, Vaccination V, AllergicSideEffect A\n" +
                    "WHERE S.effectcode = A.effectcode AND A.effectname = '" + effectname + "' AND V.code = S.code AND V.userID = U.userID # change the loss of speech to the variable\n" +
                    "))\n" +
                    "ORDER BY U.userID;");
            while (resultSet.next()) s += 1;  // Go to the end
            QueryResult.UserIDuserNameAddressResult[] res = new QueryResult.UserIDuserNameAddressResult[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                QueryResult.UserIDuserNameAddressResult tmp = new QueryResult.UserIDuserNameAddressResult(resultSet.getString("userID"), resultSet.getString("userName"), resultSet.getString("address"));
                res[i] = tmp;
                i += 1;
            }
            return res;
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.7 error!");
        }
        return new QueryResult.UserIDuserNameAddressResult[0];
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersWithAtLeastTwoDifferentVaccineTypeByGivenInterval(String startdate, String enddate) {
        Statement statement = null;
        int s = 0;
        try{
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT DISTINCT U.userID, U.userName, U.address\n" +
                    "FROM User U, Vaccination V1, Vaccination V2\n" +
                    "WHERE V1.code <> V2.code AND V1.userID = U.userID AND V2.userID = U.userID\n" +
                    " AND V1.vacdate <= '" + enddate + "' AND V1.vacdate >= '" + startdate + "'\n" +
                    "  AND V2.vacdate <= '" + enddate + "' AND V2.vacdate >= '" + startdate + "'\n" +
                    "\n" +
                    " ORDER BY U.userID;");
            while (resultSet.next()) s += 1;  // Go to the end
            QueryResult.UserIDuserNameAddressResult[] res = new QueryResult.UserIDuserNameAddressResult[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                QueryResult.UserIDuserNameAddressResult tmp = new QueryResult.UserIDuserNameAddressResult(resultSet.getString("userID"), resultSet.getString("userName"), resultSet.getString("address"));
                res[i] = tmp;
                i += 1;
            }
            return res;
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.8 error!");
        }
        return new QueryResult.UserIDuserNameAddressResult[0];
    }

    @Override
    public AllergicSideEffect[] getSideEffectsOfUserWhoHaveTwoDosesInLessThanTwentyDays() {
        Statement statement = null;
        int s = 0;
        try{
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT U1.userID, A.effectcode, A.effectname\n" +
                    "FROM User U1, User U2, Seen S, Vaccination V1, Vaccination V2, AllergicSideEffect A\n" +
                    "WHERE U1.userID = U2.userID AND U1.userID = V1.userID AND V1.code = V2.code AND V1.userID = V2.userID AND V1.dose = (V2.dose - 1)  AND DATEDIFF(V2.vacdate, V1.vacdate) < 20 AND S.userID = U1.userID AND S.effectcode = A.effectcode\n" +
                    "ORDER BY A.effectcode;");
            while (resultSet.next()) s += 1;  // Go to the end
            AllergicSideEffect[] res = new AllergicSideEffect[s];  // Alocate the array
            while(resultSet.previous()) ; // Go back to the start
            int i = 0;
            while (resultSet.next())
            {
                AllergicSideEffect tmp = new AllergicSideEffect(resultSet.getInt("effectcode"), resultSet.getString("effectname"));
                res[i] = tmp;
                i += 1;
            }
            return res;
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.9 error!");
        }
        return new AllergicSideEffect[0];
    }

    @Override
    public double averageNumberofDosesofVaccinatedUserOverSixtyFiveYearsOld() {
        Statement statement = null;
        double s = 0;
        try{
            statement = con.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT AVG(N.dose)\n" +
                    "FROM (SELECT COUNT(V.dose) as dose\n" +
                    "FROM Vaccination V, User U\n" +
                    "WHERE V.userID = U.userID AND U.age > 65\n" +
                    "GROUP BY U.userID) AS N");
            resultSet.first();
            s = resultSet.getDouble(1);
            return s;
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.10 error!");
        }
        return 0;
    }

    @Override
    public int updateStatusToEligible(String givendate) {
        Statement statement = null;
        int s = 0;
        try{
            statement = con.createStatement();
            return statement.executeUpdate("UPDATE User\n" +
                    "SET status = 'Eligible'\n" +
                    "WHERE status = 'Not_Eligible' AND userID IN \n" +
                    "\t\t\t\t(SELECT V.userID\n" +
                    "\t\t\t\t FROM Vaccination V\n" +
                    "\t\t\t\t WHERE (V.vacdate,V.userID) IN (SELECT MAX(V2.vacdate), V2.userID FROM Vaccination V2 GROUP BY V2.userID) AND DATEDIFF('" + givendate + "', V.vacdate)>120);");
        } catch (SQLException e) {
            e.printStackTrace();
            System.out.println("3.11 error!");
        }
        return 0;
    }

    @Override
    public Vaccine deleteVaccine(String vaccineName) {
        Statement statement = null;
        int s = 0;
        try
        {
            statement = con.createStatement();
            ResultSet resultSet1 = statement.executeQuery("SELECT * FROM Vaccine V WHERE V.vaccinename = '" + vaccineName + "';");
            resultSet1.first();  // Just for fixing an error.
            Vaccine vac = new Vaccine(resultSet1.getInt("code"), resultSet1.getString("vaccineName"), resultSet1.getString("type"));
            statement.executeUpdate("DELETE FROM Vaccine WHERE vaccinename = '" + vaccineName + "';");
            return vac;
            /*
            ResultSet resultSet = statement.executeQuery("SELECT COUNT(*) FROM Vaccine");
            s = resultSet.getInt(1); // s is the # of rows of Vaccine table*/
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("3.12 error!");
        }
        return null;
    }

   /*
   public static void main(String[] args)
    {
        CENGVACDB cengvac = new CENGVACDB();
        cengvac.initialize();
    }
    */
}

