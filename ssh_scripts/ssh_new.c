/**Creating Session and setting options*/
#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>


//Function to execute a remote command and print the result
int executing_remote_command(ssh_session session, const char *command) {
    ssh_channel channel;
    int rc;
    char buffer[256];
    int nbytes;

    //Create a new SSH channel
    channel = ssh_channel_new(session);
    if (channel == NULL)
       return SSH_ERROR;

    //Open SSH Channel
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        return rc;
    }

    //Execute the command on the remote server
    rc = ssh_channel_request_exec(channel, command);
    if (rc !=SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return rc;
    }

    //Read and print the command's output
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        if (write(1, buffer, nbytes) != (unsigned int) nbytes)
        {
            ssh_channel_close(channel);
            ssh_channel_free(channel);
            return SSH_ERROR;
        }
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)
    }

    //Error Check for Reading
    if (nbytes < 0) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return SSH_ERROR;
    }

    //Clean up the SSH Channel
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return SSH_OK;
}


int main()
{
    ssh_session my_ssh_session;
    int rc;
    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL){
         exit(-1);
    }

    
    //Setting up SSH options, such as the host and the user
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "192.168.0.7");
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "msfadmin");

    //Connect to the remote server
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to host: %s\n", ssh_get_error(my_ssh_session));
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }

    //Attempting public key authentication using the private key
    rc = ssh_userauth_publickey_auto(my_ssh_session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authentication with public key: %s\n", ssh_get_error(my_ssh_session));
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1)
    }

    //Execute multiple command
    const char *command = {"ls", "ls -la", "whoami", "df -h", "uptime"};
    for (i=0, i < sizeof(commands) / sizeof(commands[0]); ++i) {
        if (rc != SSH_OK) {
            fprintf(stderr, "Error execting command: %s\n", commands[i]);
        }
        
        printf("\n");

    }

    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);

    return 0;

}