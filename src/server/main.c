#include <thrift/c_glib/server/thrift_server.h>
#include <thrift/c_glib/server/thrift_simple_server.h>
#include <thrift/c_glib/transport/thrift_buffered_transport_factory.h>
#include <thrift/c_glib/transport/thrift_server_socket.h>
#include <thrift/c_glib/transport/thrift_server_transport.h>
#include <thrift/c_glib/protocol/thrift_protocol_factory.h>
#include <thrift/c_glib/protocol/thrift_binary_protocol_factory.h>

#include "handler.h"

G_BEGIN_DECLS

#define TYPE_IMPL_QUERY_EXECUTOR_HANDLER \
  (impl_query_executor_handler_get_type ())

#define IMPL_QUERY_EXECUTOR_HANDLER(obj)                                \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj),                                   \
                               TYPE_IMPL_QUERY_EXECUTOR_HANDLER,        \
                               ImplQueryExecutorHandler))
#define IMPL_QUERY_EXECUTOR_HANDLER_CLASS(c)                    \
  (G_TYPE_CHECK_CLASS_CAST ((c),                                \
                            TYPE_IMPL_QUERY_EXECUTOR_HANDLER,   \
                            ImplQueryExecutorHandlerClass))
#define IS_IMPL_QUERY_EXECUTOR_HANDLER(obj)                             \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj),                                   \
                               TYPE_IMPL_QUERY_EXECUTOR_HANDLER))
#define IS_IMPL_QUERY_EXECUTOR_HANDLER_CLASS(c)                 \
  (G_TYPE_CHECK_CLASS_TYPE ((c),                                \
                            TYPE_IMPL_QUERY_EXECUTOR_HANDLER))
#define IMPL_QUERY_EXECUTOR_HANDLER_GET_CLASS(obj)              \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                            \
                              TYPE_IMPL_QUERY_EXECUTOR_HANDLER, \
                              ImplQueryExecutorHandlerClass))

struct _ImplQueryExecutorHandler
{
    QueryExecutorHandler parent_instance;

    State state;
};
typedef struct _ImplQueryExecutorHandler ImplQueryExecutorHandler;


struct _ImplQueryExecutorHandlerClass
{
    QueryExecutorHandlerClass parent_class;
};
typedef struct _ImplQueryExecutorHandlerClass ImplQueryExecutorHandlerClass;

GType impl_query_executor_handler_get_type(void);

G_END_DECLS

G_DEFINE_TYPE(ImplQueryExecutorHandler, impl_query_executor_handler, TYPE_QUERY_EXECUTOR_HANDLER)


char *storage_file_name;

gboolean impl_query_executor_handler_execute(QueryExecutorIf *interface, ProtocolQueryResult **result, const ProtocolQuery *query, GError **error);
gboolean impl_query_executor_handler_execute(QueryExecutorIf *interface, ProtocolQueryResult **result, const ProtocolQuery *query, GError **error)
{
    printf("got request\n");
    ImplQueryExecutorHandler *self;
    self = IMPL_QUERY_EXECUTOR_HANDLER(interface);

    Maybe_Nothing mb;
    uint8_t is_err = 0;
    char *err_msg;

    printf("point 1\n");

    switch (query->type)
    {
        case PROTOCOL_QUERY_TYPE_SELECT:
            mb = handle_select_query(&self->state, result, query->query->select);
            if (!mb.ok)
            {
                is_err = 1;
                err_msg = g_strdup(mb.exception);
            }
            break;
        case PROTOCOL_QUERY_TYPE_INSERT:
            mb = handle_insert_query(&self->state, result, query->query->insert);
            if (!mb.ok)
            {
                is_err = 1;
                err_msg = g_strdup(mb.exception);
            }
            break;
        case PROTOCOL_QUERY_TYPE_DELETE:
            mb = handle_delete_query(&self->state, result, query->query->delete);
            if (!mb.ok)
            {
                is_err = 1;
                err_msg = g_strdup(mb.exception);
            }
            break;
        case PROTOCOL_QUERY_TYPE_UPDATE:
            mb = handle_update_query(&self->state, result, query->query->update);
            if (!mb.ok)
            {
                is_err = 1;
                err_msg = g_strdup(mb.exception);
            }
            break;
        case PROTOCOL_QUERY_TYPE_CREATE_TABLE:
            mb = handle_create_table_query(&self->state, result, query->query->create_table);
            if (!mb.ok)
            {
                is_err = 1;
                err_msg = g_strdup(mb.exception);
            }
            break;
        case PROTOCOL_QUERY_TYPE_DELETE_TABLE:
            mb = handle_delete_table_query(&self->state, result, query->query->delete_table);
            if (!mb.ok)
            {
                is_err = 1;
                err_msg = g_strdup(mb.exception);
            }
            break;
    }

    printf("point 2\n");

    if (is_err)
    {
        printf("setting err\n");
        g_object_set
        (
            *result,
            "code", PROTOCOL_QUERY_RESULT_CODE_ERROR,
            "error_message", g_strdup(err_msg),
            NULL
        );
    }
    return 1;
}

static void impl_query_executor_handler_init(ImplQueryExecutorHandler *self)
{
    self->state = TRY(State, storage_connect(storage_file_name));
}

static void impl_query_executor_handler_finalize(GObject *obj)
{
    ImplQueryExecutorHandler *self = IMPL_QUERY_EXECUTOR_HANDLER(obj);
    TRY(Nothing, storage_disconnect(&self->state));
    G_OBJECT_CLASS(impl_query_executor_handler_parent_class)->finalize(obj);
}

static void impl_query_executor_handler_class_init(ImplQueryExecutorHandlerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    QueryExecutorHandlerClass *handler = QUERY_EXECUTOR_HANDLER_CLASS(klass);
    handler->execute = impl_query_executor_handler_execute;
    gobject_class->finalize = impl_query_executor_handler_finalize;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("usage: lab3-server [data_file_name] [port]\n");
        return 1;
    }
    storage_file_name = argv[1];
    int port = atoi(argv[2]);

    ImplQueryExecutorHandler *handler;
    QueryExecutorProcessor *processor;

    ThriftServerTransport *server_transport;
    ThriftTransportFactory *transport_factory;
    ThriftProtocolFactory *protocol_factory;

    ThriftServer *server = NULL;
    GError *error = NULL;

    handler = g_object_new(TYPE_IMPL_QUERY_EXECUTOR_HANDLER, NULL);

    processor = g_object_new
    (
        TYPE_QUERY_EXECUTOR_PROCESSOR,
        "handler", handler,
        NULL
    );

    server_transport = g_object_new
    (
        THRIFT_TYPE_SERVER_SOCKET,
        "port", port,
        NULL
    );
    transport_factory = g_object_new(THRIFT_TYPE_BUFFERED_TRANSPORT_FACTORY, NULL);
    protocol_factory = g_object_new(THRIFT_TYPE_BINARY_PROTOCOL_FACTORY, NULL);

    server = g_object_new
    (
        THRIFT_TYPE_SIMPLE_SERVER,
        "processor", processor,
        "server_transport", server_transport,
        "input_transport_factory", transport_factory,
        "output_transport_factory", transport_factory,
        "input_protocol_factory",   protocol_factory,
        "output_protocol_factory",  protocol_factory,
        NULL
    );

    printf("starting the server...\n");
    thrift_server_serve(server, &error);
}