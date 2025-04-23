```c
// Sockets: ¿Qué son y cómo funcionan?

Un socket es como un tubo que conecta dos puntos: el cliente y el servidor. El servidor espera en un lugar (un puerto específico) a que el cliente se conecte. Una vez que ambos están conectados, pueden enviarse datos de un lado al otro.

- **Servidor**: Es quien “escucha” y está esperando que un cliente lo contacte. Piensa en el servidor como un recepcionista que siempre está disponible para ayudar a los clientes.
- **Cliente**: El cliente es quien inicia la conversación conectándose al servidor. Es como si estuviera llamando al recepcionista para pedir información.

Ejemplo:
- **Servidor**: "Hola, estoy esperando en el puerto 8080. ¿Hay algún cliente que quiera conectarse?"
- **Cliente**: "Sí! Quiero conectarme al puerto 8080 para hablar contigo, dámelo to!"

--------------------------------
// Crear un socket: Primer paso para la comunicación

int socket(int domain, int type, int protocol);

Esta función crea el socket, que es el tubo a través del cual el cliente y el servidor se comunicarán.

- **domain**: Especifica el tipo de direcciones con las que vamos a trabajar.
    - `AF_INET`: Esto indica que usaremos direcciones IPv4 (como 192.168.1.1).
    - `AF_INET6`: Para direcciones IPv6, un formato más reciente de direcciones IP.

- **type**: Define cómo vamos a comunicarnos.
    - `SOCK_STREAM`: Para conexiones TCP. TCP garantiza que los datos lleguen en el orden correcto y sin pérdida.
    - `SOCK_DGRAM`: Para conexiones UDP. UDP es más rápido, pero no garantiza que los datos lleguen o estén en el orden correcto.
	
- **protocol**: Generalmente se pone en 0 para que el sistema elija el protocolo adecuado basado en el tipo de socket. En nuestro caso, TCP.

```c
int server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket == -1) {
    perror("Error al crear el socket");
    // Si hay un error, perror imprime una descripción del problema basado en errno.
}
```
En este ejemplo, estamos creando un socket para conexiones TCP con direcciones IPv4.

--------------------------------
// Configurar opciones del socket con setsockopt()

```c
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```
Esta función ajusta algunas opciones del socket para mejorar su comportamiento.

- **sockfd**: El descriptor del socket (el número que representa nuestro socket).
- **level**: Es el nivel al que se aplicará la opción. Usamos `SOL_SOCKET` para aplicar opciones que son comunes a todos los sockets, sin importar el protocolo.
- **optname**: La opción específica que queremos ajustar. En este caso, usamos `SO_REUSEADDR`.
    - **SO_REUSEADDR**: Permite que el servidor reutilice rápidamente la misma dirección (IP y puerto), incluso si acaba de cerrar el socket. Sin esta opción, tendríamos que esperar un tiempo antes de reutilizar esa dirección.
- **optval**: Es un puntero que apunta a la opción que estamos configurando. Para `SO_REUSEADDR`, lo que hacemos es pasarle un valor de `1` para activarlo.
- **optlen**: El tamaño de la opción.

```c
int opt = 1;
setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```
Esto permite que el servidor cierre y vuelva a abrir rápidamente sin esperar.

--------------------------------
// Sockets no bloqueantes con fcntl()

Un socket puede configurarse como **bloqueante** o **no bloqueante**. 

- **Bloqueante**: Significa que cuando el servidor intenta leer o escribir en el socket, se quedará esperando hasta que haya datos disponibles. Esto puede hacer que el programa se detenga si no hay datos en ese momento.
- **No bloqueante**: El servidor no se detiene si no hay datos disponibles. Continúa con otras tareas y revisa más tarde si hay datos.

Usamos la función `fcntl()` para hacer el socket no bloqueante:

```c
#include <fcntl.h>
fcntl(server_socket, F_SETFL, O_NONBLOCK);
```
Esto es importante en servidores que manejan muchos clientes, porque el servidor puede seguir atendiendo a otros clientes aunque uno de ellos no esté listo para comunicarse.

--------------------------------
// sockaddr_in: Direcciones y puertos

La estructura `sockaddr_in` se usa para definir las direcciones y puertos en un socket. Es la forma en que especificamos en qué dirección IP y puerto queremos que el servidor escuche.

```c
struct sockaddr_in {
    sa_family_t    sin_family;   // Tipo de dirección (IPv4)
    in_port_t      sin_port;     // Número de puerto
    struct in_addr sin_addr;     // Dirección IP
    char           sin_zero[8];  // Relleno
};
```
- **sin_family**: Debe ser siempre `AF_INET` para IPv4.
- **sin_port**: El puerto que el servidor usará para escuchar. Se convierte al formato "big-endian" con la función `htons()`, que asegura que el número del puerto se almacene de forma correcta en la red.
- **sin_addr**: La dirección IP donde queremos que el servidor escuche. Usamos `INADDR_ANY` para que acepte conexiones desde cualquier dirección IP.
- **sin_zero**: Simplemente es un relleno que asegura que la estructura tenga el tamaño correcto. No lo utilizamos.

Ejemplo:
```c
struct sockaddr_in server_addr;
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080); // Escuchar en el puerto 8080
server_addr.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones desde cualquier IP
memset(&(server_addr.sin_zero), 0, 8); // Relleno a 0
```

--------------------------------
// Asociar socket a una dirección con bind()

Después de configurar la dirección y el puerto, necesitamos asociar el socket a esa dirección. Lo hacemos con `bind()`:

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

- **sockfd**: El descriptor del socket.
- **addr**: La dirección (en este caso, `server_addr`).
- **addrlen**: El tamaño de la estructura `sockaddr_in`.

Ejemplo:
```c
if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("Error en bind");
}
```
Esto le dice al sistema operativo que el servidor escuchará en la IP y puerto que hemos especificado.

```c
// Escuchar conexiones con listen()

Una vez que el socket está configurado y asociado a una dirección, el servidor puede empezar a escuchar conexiones entrantes con `listen()`:

```c
int listen(int sockfd, int backlog);
```

- **sockfd**: El descriptor del socket.
- **backlog**: El número máximo de conexiones en cola que pueden estar esperando para ser aceptadas.

```c
if (listen(server_socket, 10) == -1) {
    perror("Error en listen");
}
```

En este ejemplo, el servidor puede manejar hasta 10 conexiones en cola antes de rechazar nuevas conexiones.

--------------------------------
// Aceptar conexiones con accept()

Cuando un cliente intenta conectarse al servidor, el servidor acepta la conexión con la función `accept()`:

```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- **sockfd**: El descriptor del socket en el que estamos escuchando.
- **addr**: Un puntero a una estructura `sockaddr_in` que almacenará la información del cliente (su dirección IP y puerto).
- **addrlen**: El tamaño de la estructura `sockaddr_in`.

```c
struct sockaddr_in client_addr;
socklen_t client_addr_size = sizeof(client_addr);
int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
if (client_socket == -1) {
    perror("Error en accept");
}
```

Con esto, el servidor ha aceptado la conexión de un cliente y tiene un nuevo socket (`client_socket`) dedicado a comunicarse con ese cliente en particular.

--------------------------------
// Leer y enviar datos: recv() y send()

Una vez que el servidor ha aceptado una conexión, puede empezar a recibir y enviar datos a través del nuevo socket del cliente.

- **recv()**: Recibe datos desde el cliente.
- **send()**: Envía datos al cliente.

```c
int recv(int sockfd, void *buf, size_t len, int flags);
int send(int sockfd, const void *buf, size_t len, int flags);
```

Ejemplo de lectura de datos del cliente:
```c
char buffer[1024];
int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
if (bytes_received == -1) {
    perror("Error en recv");
} else {
    buffer[bytes_received] = '\0'; // Asegurarse de que el buffer termine en un null
    printf("Mensaje recibido: %s\n", buffer);
}
```

Ejemplo de envío de datos al cliente:
```c
char *message = "¡Hola cliente!";
send(client_socket, message, strlen(message), 0);
```

--------------------------------
// Cerrar conexiones con close()

Cuando el servidor ha terminado de comunicarse con el cliente, debe cerrar el socket para liberar recursos.

```c
close(client_socket);
```

--------------------------------
// Multiplexación de conexiones: poll()

Cuando se manejan múltiples conexiones al mismo tiempo, es ineficiente usar un socket bloqueante para cada cliente. Aquí es donde entra en juego `poll()`, que permite al servidor monitorear varios sockets y saber cuál de ellos tiene datos disponibles.

```c
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

- **fds**: Es un array de estructuras `pollfd`, donde cada elemento representa un socket.
- **nfds**: El número de elementos en el array `fds`.
- **timeout**: Cuánto tiempo esperar por una actividad. Si es `-1`, esperará indefinidamente.

Ejemplo:
```c
struct pollfd fds[10]; // Monitorear hasta 10 sockets
fds[0].fd = server_socket; // El socket del servidor
fds[0].events = POLLIN; // Queremos monitorear si hay datos para leer (POLLIN)

int poll_count = poll(fds, 1, -1);
if (poll_count == -1) {
    perror("Error en poll");
}
```

En este ejemplo, el servidor esperará indefinidamente a que haya actividad en el `server_socket`. Cuando `poll()` retorna, el servidor sabrá qué socket tiene datos listos para ser procesados.

```

```c
// Gestionar múltiples clientes de forma eficiente

Una vez que tu servidor ha aceptado conexiones de varios clientes, debes manejar las comunicaciones de forma eficiente para que el servidor no se bloquee cuando un cliente no envía datos inmediatamente.

1. **No bloquear las operaciones de E/S**:
   Usa sockets en modo no bloqueante, como se mencionó anteriormente, para evitar que las operaciones de lectura o escritura bloqueen al servidor mientras esperan datos de un cliente.
   
   Configurar un socket como no bloqueante:
   ```c
   int flags = fcntl(client_socket, F_GETFL, 0);
   fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
   ```

2. **Multiplexar conexiones**:
   Ya has visto `poll()`, que permite monitorear múltiples sockets al mismo tiempo. Sin embargo, cuando tienes muchos clientes, podrías considerar utilizar **epoll()** (en Linux) o **kqueue** (en sistemas BSD) para mayor eficiencia si el número de conexiones es muy grande. `poll()` es suficiente para la mayoría de los casos, pero en entornos con muchas conexiones simultáneas, otras alternativas pueden ser más rápidas.

--------------------------------
// Protocolo de mensajes en IRC

Los servidores IRC siguen un protocolo de mensajes estándar. Aquí es donde defines cómo se formatean los mensajes que envían y reciben tanto el servidor como los clientes. Los mensajes en IRC tienen una estructura específica:

- Los mensajes IRC tienen tres partes: un prefijo opcional, un comando y uno o más parámetros.
- Formato básico de un mensaje:
  
  ```plaintext
  :<prefix> <command> <params> :<trailing>
  ```

  Ejemplo de mensaje para unirse a un canal:
  ```plaintext
  JOIN #canal
  ```

- **JOIN**: Comando para que un usuario se una a un canal.
- **PRIVMSG**: Comando para enviar un mensaje privado a un usuario o canal.
  
  ```c
  char *join_msg = "JOIN #canal\r\n";
  send(client_socket, join_msg, strlen(join_msg), 0);
  ```

  Ejemplo de mensaje privado:
  ```plaintext
  PRIVMSG usuario :Hola, ¿cómo estás?
  ```

  Un mensaje con prefijo podría indicar el origen del mensaje, como cuando el servidor responde a un comando enviado por el cliente:

  ```plaintext
  :servidor 001 usuario :Bienvenido al servidor IRC
  ```

--------------------------------
// Desconectar clientes y limpiar recursos

Cuando un cliente se desconecta (ya sea de forma voluntaria o por un error), es importante cerrar correctamente el socket y limpiar cualquier recurso relacionado. Si no haces esto, podrías quedarte con "file descriptors" abiertos y consumir memoria innecesariamente.

- Cierra el socket:
  ```c
  close(client_socket);
  ```

- Remueve el cliente de la lista de sockets en `poll()` o `epoll()` para evitar errores en futuras llamadas.

--------------------------------
// Gestión de señales (opcional)

En un servidor en red, es útil gestionar señales del sistema, como **SIGINT** (Ctrl+C) para poder realizar una salida controlada cuando el programa se detiene.

- **signal()**: Captura señales y ejecuta una función específica cuando ocurre la señal.
  ```c
  #include <signal.h>

  void signal_handler(int signal) {
      if (signal == SIGINT) {
          printf("Servidor detenido.\n");
          close(server_socket);
          exit(0);
      }
  }

  signal(SIGINT, signal_handler);
  ```

Esto permite que el servidor cierre de manera ordenada cuando recibe una señal para detenerse, evitando así problemas como sockets no cerrados adecuadamente o datos no enviados.

--------------------------------
// Implementación del bucle principal del servidor

El bucle principal del servidor IRC, después de aceptar conexiones y configurar `poll()` para manejar múltiples clientes, se verá algo así:

```c
while (1) {
    int poll_count = poll(fds, nfds, -1);
    if (poll_count == -1) {
        perror("Error en poll");
        break;
    }

    for (int i = 0; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
            if (fds[i].fd == server_socket) {
                // Aceptar una nueva conexión de cliente
                int new_client = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
                if (new_client == -1) {
                    perror("Error en accept");
                    continue;
                }
                fds[nfds].fd = new_client;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                // Manejar datos de un cliente existente
                char buffer[1024];
                int bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    // El cliente cerró la conexión o hubo un error
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1];  // Remover el cliente de la lista
                    nfds--;
                } else {
                    buffer[bytes_received] = '\0';
                    printf("Mensaje del cliente: %s\n", buffer);
                    // Procesar el mensaje del cliente...
                }
            }
        }
    }
}
```

Este bucle permite al servidor aceptar nuevas conexiones, leer datos de los clientes y gestionar desconexiones de manera eficiente.
```
```c
// Autenticación de usuarios y gestión de nicks

Una parte clave del protocolo IRC es la autenticación de usuarios y la asignación de nicks (nombres de usuario únicos). Normalmente, al conectarse al servidor, un cliente IRC debe registrarse con un comando **NICK** y un comando **USER**.

- **NICK**: Asigna un apodo (nickname) al cliente.
  ```plaintext
  NICK apodo
  ```

  Si un apodo ya está en uso, el servidor debería notificar al cliente con un mensaje de error:
  ```plaintext
  :servidor 433 * apodo :Nickname is already in use
  ```

- **USER**: Proporciona el nombre de usuario y otra información. Generalmente, el comando tiene el formato:
  ```plaintext
  USER nombre_usuario * * :nombre_real
  ```

  Ejemplo en código:
  ```c
  char *nick_msg = "NICK mi_apodo\r\n";
  send(client_socket, nick_msg, strlen(nick_msg), 0);

  char *user_msg = "USER mi_nombre_usuario * * :Nombre Real\r\n";
  send(client_socket, user_msg, strlen(user_msg), 0);
  ```

  Deberás implementar una estructura de datos que rastree a los usuarios conectados y sus nicks para asegurarte de que no haya duplicados.

--------------------------------
// Canales en IRC

Los **canales** son una parte fundamental del protocolo IRC, y los clientes se pueden unir a ellos para chatear en grupo. Los canales son identificados por un **#** seguido del nombre del canal, por ejemplo, `#general`.

- Un cliente se une a un canal con el comando **JOIN**:
  ```plaintext
  JOIN #general
  ```

- El servidor debería mantener una lista de todos los canales y los usuarios que están en cada uno.

Ejemplo de cómo puedes estructurar los canales:
```c
struct Channel {
    char name[50];
    int clients[MAX_CLIENTS];
    int client_count;
};
```

- Los clientes pueden enviar mensajes a todos los miembros del canal con el comando **PRIVMSG**.
  ```plaintext
  PRIVMSG #general :Hola a todos en el canal
  ```

--------------------------------
// Comandos adicionales útiles en IRC

Además de **JOIN** y **PRIVMSG**, hay otros comandos que son esenciales para el funcionamiento de un servidor IRC:

- **PART**: Permite a un usuario salir de un canal.
  ```plaintext
  PART #general
  ```

- **QUIT**: Desconecta al usuario del servidor IRC. Cuando un usuario envía **QUIT**, todos los demás usuarios y canales deben ser notificados de la desconexión.
  ```plaintext
  QUIT :Razón de la desconexión
  ```

- **PING/PONG**: Estos comandos son utilizados para verificar la conexión entre el cliente y el servidor. El servidor envía un **PING**, y el cliente debe responder con un **PONG** para mantener la conexión activa.
  ```plaintext
  PING :servidor
  PONG :servidor
  ```

  Implementación de un manejo básico de PING/PONG:
  ```c
  if (strncmp(buffer, "PING", 4) == 0) {
      char pong_msg[1024];
      snprintf(pong_msg, sizeof(pong_msg), "PONG %s\r\n", buffer + 5);
      send(client_socket, pong_msg, strlen(pong_msg), 0);
  }
  ```

--------------------------------
// Manejo de errores comunes

Al desarrollar un servidor IRC, te encontrarás con varios errores potenciales que deberás manejar de manera adecuada:

1. **Nickname en uso**: Ya mencionado anteriormente, si un usuario intenta usar un nick que ya está en uso, se debe enviar un mensaje de error apropiado.

2. **Errores de conexión**: Si la conexión con un cliente falla (por ejemplo, el cliente se desconecta inesperadamente), asegúrate de liberar todos los recursos asociados al cliente y notificar al resto de los usuarios en los canales correspondientes.

3. **Mensajes mal formateados**: El servidor debe ser capaz de manejar y rechazar mensajes mal formateados o comandos inválidos. Por ejemplo:
   ```plaintext
   :servidor 461 NICK :Not enough parameters
   ```

--------------------------------
// Seguridad y límites en el servidor IRC

1. **Límites en el tamaño de los mensajes**: Deberías establecer un límite en el tamaño de los mensajes que los clientes pueden enviar para evitar desbordamientos de búfer. Un límite común es 512 bytes por mensaje (incluyendo el CRLF final).

2. **Límites de usuarios**: Puedes establecer un número máximo de conexiones simultáneas en el servidor para evitar que tu servidor se sobrecargue. Esto se puede hacer fácilmente con un array de sockets que monitorees con `poll()`.

3. **Timeouts**: Para evitar que un cliente bloquee indefinidamente el servidor sin enviar datos, puedes implementar un timeout usando `poll()`. Si un cliente no envía datos en un tiempo determinado, se puede desconectar automáticamente.

```c
struct pollfd fds[MAX_CLIENTS];
int timeout_msecs = 30000;  // Timeout de 30 segundos
int poll_count = poll(fds, nfds, timeout_msecs);
if (poll_count == 0) {
    printf("Timeout: cliente desconectado por inactividad\n");
    close(client_socket);
}
```

