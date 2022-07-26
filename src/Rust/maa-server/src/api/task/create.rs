use super::super::{Error, MaaManager};
use actix_web::{web, HttpResponse, Responder};
use serde::Deserialize;
use serde_json::{json, Value};
use std::sync::Mutex;

#[allow(dead_code)]
#[derive(Deserialize)]
pub struct Req {
    id: i64,
    types: String,
    params: Value,
}
pub async fn create(
    req: web::Json<Req>,
    maa_manager: web::Data<Mutex<MaaManager>>,
) -> Result<impl Responder, Error> {
    let task_id = {
        let mut manager = maa_manager.lock().map_err(|_| Error::Internal)?;
        let maa = manager.get_mut(req.id).ok_or(Error::InstanceNotFound)?;
        let params = match req.params {
            Value::Null => "{}".to_string(),
            Value::Object(_) => req.params.to_string(),
            _ => return Err(Error::InvaildRequest),
        };
        maa.create_task(&req.types, &params)?
    };
    Ok(HttpResponse::Ok().json(json!({
        "result":  0,
        "task_id": task_id
    })))
}
