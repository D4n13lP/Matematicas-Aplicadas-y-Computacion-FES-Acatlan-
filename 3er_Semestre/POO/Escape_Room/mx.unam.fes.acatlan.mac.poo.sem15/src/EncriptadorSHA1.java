import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class EncriptadorSHA1 {
    public static void main(String[] args) {
        String ruta = "C:\\SystemFile\\tmp~\\";

        try {
            MessageDigest md = MessageDigest.getInstance("SHA1");
            byte[] result = md.digest(ruta.getBytes());
            StringBuffer sb = new StringBuffer();
            for (byte b : result) {
                sb.append(String.format("%02x", b));
            }
            System.out.println("Ruta encriptada: " + sb.toString());
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
    }
}

