import java.util.Scanner;

public class Descifrador {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Ingresa el mensaje cifrado: ");
        String mensajeCifrado = scanner.nextLine();

        String mensajeDescifrado = descifrarMensaje(mensajeCifrado);
        System.out.println("Mensaje descifrado: " + mensajeDescifrado);
    }

    public static String descifrarMensaje(String mensajeCifrado) {
        StringBuilder mensajeDescifrado = new StringBuilder();
        int desplazamiento = 8;
        String alfabeto = "ABCDEFGHIJKLMNÑOPQRSTUVWXYZ"; // La Ñ se encuentra después de la N

        for (char caracter : mensajeCifrado.toCharArray()) {
            if (Character.isLetter(caracter)) {
                // Convertir a mayúsculas
                char mayuscula = Character.toUpperCase(caracter);
                int valor = alfabeto.indexOf(mayuscula);

                // Realizar el desplazamiento y considerar el reinicio
                valor = (valor - desplazamiento + alfabeto.length()) % alfabeto.length();

                char letraDescifrada = alfabeto.charAt(valor);
                mensajeDescifrado.append(letraDescifrada);
            }
        }
        return mensajeDescifrado.toString();
    }
}
